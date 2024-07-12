#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory>
#include <unordered_map>

#include <boost/asio/io_service.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/program_options.hpp>

#include <vanetza/btp/data_indication.hpp>
#include <vanetza/btp/data_interface.hpp>
#include <vanetza/btp/data_request.hpp>
#include <vanetza/btp/header.hpp>
#include <vanetza/btp/header_conversion.hpp>
#include <vanetza/btp/port_dispatcher.hpp>
#include <vanetza/geonet/data_confirm.hpp>
#include <vanetza/geonet/router.hpp>

#include "application.hpp"
#include "btp_service.hpp"
#include "ethernet_device.hpp"
#include "hello_application.hpp"
#include "link_layer.hpp"
#include "positioning.hpp"
#include "read_application.hpp"
#include "router_context.hpp"
#include "security.hpp"
#include "time_trigger.hpp"

namespace asio = boost::asio;
namespace gn = vanetza::geonet;
namespace po = boost::program_options;

using namespace std;
using namespace vanetza;

using DataConfirm = vanetza::geonet::DataConfirm;
using DataIndication = vanetza::btp::DataIndication;
using DataRequest = vanetza::btp::DataRequestGeoNetParams;
using DownPacketPtr = vanetza::geonet::Router::DownPacketPtr;
using PortType = vanetza::btp::port_type;
using PromiscuousHook = vanetza::btp::PortDispatcher::PromiscuousHook;
using UpPacketPtr = vanetza::geonet::Router::UpPacketPtr;

int BtpService::init(string interface) {
  po::options_description options("Allowed options");
  options.add_options()("help", "Print out available options.")(
      "link-layer,l", po::value<string>()->default_value("ethernet"),
      "Link layer type")("interface,i",
                         po::value<string>()->default_value("lo"),
                         "Network interface to use.")(
      "mac-address", po::value<string>(),
      "Override the network interface's MAC address.")(
      "require-gnss-fix",
      "Suppress transmissions while GNSS position fix is missing")(
      "gn-version", po::value<unsigned>()->default_value(1),
      "GeoNetworking protocol version to use.")(
      "cam-interval", po::value<unsigned>()->default_value(1000),
      "CAM sending interval in milliseconds.")("print-rx-cam",
                                               "Print received CAMs")(
      "print-tx-cam", "Print generated CAMs")("benchmark",
                                              "Enable benchmarking")(
      "applications,a",
      po::value<vector<string>>()->default_value({"ca"}, "ca")->multitoken(),
      "Run applications [ca,hello,benchmark]")(
      "non-strict",
      "Set MIB parameter ItsGnSnDecapResultHandling to NON_STRICT");
  add_positioning_options(options);
  add_security_options(options);
  add_link_layer_options(options);

#ifdef SOCKTAP_WITH_CUBE_EVK
  nfiniity::add_cube_evk_options(options);
#endif

  po::positional_options_description positional_options;
  positional_options.add("interface", 1);

  po::variables_map vm;

  try {
    po::store(po::command_line_parser(0, NULL)
                  .options(options)
                  .positional(positional_options)
                  .run(),
              vm);
    po::notify(vm);
  } catch (po::error &e) {
    cerr << "ERROR: " << e.what() << endl << endl;
    cerr << options << endl;
    return 1;
  }

  if (vm.count("help")) {
    cout << options << endl;
    return 1;
  }

  try {
    TimeTrigger trigger(io_service_);

    const char *device_name = interface.c_str();
    EthernetDevice device(device_name);
    MacAddress mac_address = device.address();

    if (vm.count("mac-address")) {
      cout << "Using MAC address: " << vm["mac-address"].as<string>() << "."
           << endl;

      if (!parse_mac_address(vm["mac-address"].as<string>().c_str(),
                             mac_address)) {
        cerr << "The specified MAC address is invalid." << endl;
        return 1;
      }
    }

    const string link_layer_name = vm["link-layer"].as<string>();
    auto link_layer =
        create_link_layer(io_service_, device, link_layer_name, vm);
    if (!link_layer) {
      cerr << "No link layer '" << link_layer_name << "' found." << endl;
      return 1;
    }

    auto signal_handler = [this](const boost::system::error_code &ec,
                                 int signal_number) {
      if (!ec) {
        cout << "Termination requested." << endl;
        this->io_service_.stop();
      }
    };
    asio::signal_set signals(io_service_, SIGINT, SIGTERM);
    signals.async_wait(signal_handler);

    // configure management information base
    // TODO: make more MIB options configurable by command line flags
    gn::MIB mib;
    mib.itsGnLocalGnAddr.mid(mac_address);
    mib.itsGnLocalGnAddr.is_manually_configured(true);
    mib.itsGnLocalAddrConfMethod = geonet::AddrConfMethod::Managed;
    mib.itsGnSecurity = false;
    if (vm.count("non-strict")) {
      mib.itsGnSnDecapResultHandling =
          geonet::SecurityDecapHandling::Non_Strict;
    }
    mib.itsGnProtocolVersion = vm["gn-version"].as<unsigned>();

    if (mib.itsGnProtocolVersion != 0 && mib.itsGnProtocolVersion != 1) {
      throw runtime_error("Unsupported GeoNetworking version, only version 0 "
                          "and 1 are supported.");
    }

    auto positioning =
        create_position_provider(io_service_, vm, trigger.runtime());
    if (!positioning) {
      cerr << "Requested positioning method is not available\n";
      return 1;
    }

    auto security = create_security_entity(vm, trigger.runtime(), *positioning);
    if (security) {
      mib.itsGnSecurity = true;
    }

    context_ =
        make_unique<RouterContext>(mib, trigger, *positioning, security.get());
    context_->require_position_fix(vm.count("require-gnss-fix") > 0);
    context_->set_link_layer(link_layer.get());
  } catch (PositioningException &e) {
    cerr << "Exit because of positioning error: " << e.what() << endl;
    return 1;
  } catch (exception &e) {
    cerr << "Exit: " << e.what() << endl;
    return 1;
  }

  return 0;
}

int BtpService::read(vector<uint8_t> *buff) {
  unique_ptr<ReadApplication> read{
      new ReadApplication(io_service_, chrono::milliseconds(800), buff)};
  apps_.emplace("read", move(read));

  for (const auto &app : apps_) {
    cout << "Enable application '" << app.first << "'...\n";
    context_->enable(app.second.get());
  }

  io_service_.run();
  return 0;
}

int BtpService::write(void *buf, size_t len) {
  unique_ptr<HelloApplication> hello{
      new HelloApplication(io_service_, chrono::milliseconds(800), buf, len)};
  apps_.emplace("hello", move(hello));

  for (const auto &app : apps_) {
    cout << "Enable application '" << app.first << "'...\n";
    context_->enable(app.second.get());
  }

  io_service_.run();
  return 0;
}

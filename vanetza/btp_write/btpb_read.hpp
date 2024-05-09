#include "application.hpp"
#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>
#include "ethernet_device.hpp"
#include "link_layer.hpp"
#include "positioning.hpp"
#include "router_context.hpp"
#include "security.hpp"
#include "time_trigger.hpp"
#include <boost/asio/io_service.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <vanetza/btp/data_interface.hpp>
#include <vanetza/btp/data_indication.hpp>
#include <vanetza/btp/data_request.hpp>
#include <vanetza/btp/port_dispatcher.hpp>
#include <vanetza/geonet/data_confirm.hpp>
#include <vanetza/geonet/router.hpp>
#include <vanetza/btp/header.hpp>
#include <vanetza/btp/header_conversion.hpp>
#include <cassert>
#include "read_application.hpp"
namespace asio = boost::asio;
namespace gn = vanetza::geonet;
namespace po = boost::program_options;
using namespace vanetza;
using DataConfirm = vanetza::geonet::DataConfirm;
using DataIndication = vanetza::btp::DataIndication;
using DataRequest = vanetza::btp::DataRequestGeoNetParams;
using DownPacketPtr = vanetza::geonet::Router::DownPacketPtr;
using PortType = vanetza::btp::port_type;
using PromiscuousHook = vanetza::btp::PortDispatcher::PromiscuousHook;
using UpPacketPtr = vanetza::geonet::Router::UpPacketPtr;






int btpb_read(std::string interface ,std::string &buff) {}

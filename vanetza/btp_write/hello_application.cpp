#include "hello_application.hpp"
#include <chrono>
#include <functional>
#include <iostream>
#include <vector>
// This is a very simple application that sends BTP-B messages with the content 0xc0ffee.

using namespace vanetza;

HelloApplication::HelloApplication(boost::asio::io_service& io, std::chrono::milliseconds interval, void *buf, size_t len) :
    timer_(io), interval_(interval), buff_(buf),len_(len)
{
    schedule_timer();
}

HelloApplication::PortType HelloApplication::port()
{
    return host_cast<uint16_t>(42);
}

void HelloApplication::indicate(const DataIndication& indication, UpPacketPtr packet)
{
    std::cout << "Hello application received a packet" << std::endl;
}

void HelloApplication::schedule_timer()
{
    timer_.expires_from_now(interval_);
    timer_.async_wait(std::bind(&HelloApplication::on_timer, this, std::placeholders::_1));
}

void HelloApplication::on_timer(const boost::system::error_code& ec)
{
    if (ec != boost::asio::error::operation_aborted) {
        DownPacketPtr packet { new DownPacket() };
        std::vector<unsigned char> buffer = std::vector<unsigned char>(len_);
        // printf("%02x/n",*(unsigned char*)buff_);
        // for (int i = 0; i < len_; i++) {
        //     printf("%02x ", (((unsigned char*)buff_)[i]));
        // }
        
        memcpy(buffer.data(), buff_, len_);
        packet->layer(OsiLayer::Application) = std::move(buffer);
        DataRequest request;
        request.transport_type = geonet::TransportType::SHB;
        request.communication_profile = geonet::CommunicationProfile::ITS_G5;
        request.its_aid = aid::CA;
        auto confirm = Application::request(request, std::move(packet));
        if (!confirm.accepted()) {
            throw std::runtime_error("Hello application data request failed");
        }
        throw std::runtime_error("Finish");
        schedule_timer();
    }
}

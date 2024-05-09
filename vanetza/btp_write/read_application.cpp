#include "read_application.hpp"
#include <chrono>
#include <functional>
#include <iostream>
// This is a very simple application that sends BTP-B messages with the content 0xc0ffee.

using namespace vanetza;
ReadApplication::ReadApplication(boost::asio::io_service& io, std::chrono::milliseconds interval) :
    timer_(io), interval_(interval)
{
    schedule_timer();
}

ReadApplication::PortType ReadApplication::port()
{
    return host_cast<uint16_t>(42);
}

void ReadApplication::indicate(const DataIndication& indication, UpPacketPtr packet)
{   
    
    vanetza::CohesivePacket* p = boost::get<vanetza::CohesivePacket>(packet.get());
    vanetza::ByteBuffer buff = p->buffer();
    std::string recv = "";
    for (int i = 40; i < buff.size(); i++) {
        recv = recv + char(buff[i]);
    }
    recv = recv + "\0";
    //std::cout << "Packet content: " << p->buffer() << std::endl;
    throw std::runtime_error(recv);

}

void ReadApplication::schedule_timer()
{
    timer_.expires_from_now(interval_);
    timer_.async_wait(std::bind(&ReadApplication::on_timer, this, std::placeholders::_1));
}

void ReadApplication::on_timer(const boost::system::error_code& ec)
{
    if (ec != boost::asio::error::operation_aborted) {
        
        schedule_timer();
    }
}

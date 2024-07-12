#pragma once

#include "application.hpp"
#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>

class HelloApplication : public Application
{
public:
    HelloApplication(boost::asio::io_service&, std::chrono::milliseconds interval , void *buf, size_t len);
    PortType port() override;
    void indicate(const DataIndication&, UpPacketPtr) override;

private:
    void schedule_timer();
    void on_timer(const boost::system::error_code& ec);

    boost::asio::steady_timer timer_;
    std::chrono::milliseconds interval_;

    void *buff_;
    size_t len_;
};

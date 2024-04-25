#ifndef HELLO_APPLICATION_HPP_EUIC2VFR
#define HELLO_APPLICATION_HPP_EUIC2VFR

#include "application.hpp"
#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>

class HelloApplication : public Application
{
public:
    HelloApplication(boost::asio::io_service&, std::chrono::milliseconds interval , std::string buff);
    PortType port() override;
    void indicate(const DataIndication&, UpPacketPtr) override;

private:
    void schedule_timer();
    void on_timer(const boost::system::error_code& ec);

    boost::asio::steady_timer timer_;
    std::chrono::milliseconds interval_;
    std::string buff;
};

#endif /* HELLO_APPLICATION_HPP_EUIC2VFR */

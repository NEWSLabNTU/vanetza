#pragma once

#include <cstddef>
#include <unordered_map>
#include <string>

#include <boost/asio/io_service.hpp>
#include <vanetza/geonet/router.hpp>

#include "router_context.hpp"

using namespace std;
using namespace vanetza;

class BtpService {
public:
    int init(string interface);
    int read(vector<uint8_t> *buff);
    int write(void *buf, size_t len);

private:
    boost::asio::io_service io_service_;
    unique_ptr<RouterContext> context_;
    unordered_map<string, unique_ptr<Application>> apps_;
};

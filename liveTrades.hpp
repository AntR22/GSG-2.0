#pragma once

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/beast/core/ostream.hpp>
#include <string>
#include <utility>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <iostream>
#include <iomanip>
#include <ctime>
#include "JSONParser.hpp"
#include "data.hpp"
#include "indicators.hpp"
#include <stdexcept>


#define ONEHOUR_ONEMONTH 672
#define ONEMIN_ONEWEEK 10080
#define ONESEC_ONEDAY 86400
#define ONEMIN_ONEHOUR 60

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp; 

inline int marketStream() {
    try {
        std::time_t t = std::time(nullptr);
        std::tm tm = *std::localtime(&t);

        cData testing(ONEMIN_ONEHOUR);
        volumeProfile testvolume(0.01, 1000.0, false, 0.01);
        // WebSocket endpoint
        std::string host = "stream.binance.com";
        std::string port = "443";
        // Create the I/O context
        boost::asio::io_context ioc;

        // Creates SSL context and holds certificate
        ssl::context ctx{ssl::context::tlsv12_client};
    
        tcp::resolver resolver(ioc);
        // Create the WebSocket stream
        websocket::stream<beast::ssl_stream<tcp::socket>> ws{ioc, ctx};

        // Resolve the hostname
        auto endpoints = resolver.resolve(host, port);

        // Connect to the first endpoint in the list
        auto ep = net::connect(get_lowest_layer(ws), endpoints);
        
        //Set SNI HostName
        if(! SSL_set_tlsext_host_name(ws.next_layer().native_handle(), host.c_str()))
            throw beast::system_error(
                beast::error_code(
                    static_cast<int>(::ERR_get_error()),
                    net::error::get_ssl_category()),
                "Failed to set SNI Hostname");
        host += ':' + std::to_string(ep.port());

        ws.next_layer().handshake(ssl::stream_base::client);

        ws.set_option(websocket::stream_base::decorator(
            [](websocket::request_type& req)
            {
                req.set(http::field::user_agent,
                    std::string(BOOST_BEAST_VERSION_STRING) +
                        " websocket-client-coro");
            }));

        boost::beast::error_code ec;

        //ws.handshake(host, "/ws/ethusdt@kline_1m", ec);
        ws.handshake(host, "/ws/ethusdt@aggTrade", ec);

        if (ec) {
            std::cerr << "Error with handshake: " << ec.message() << std::endl;
            return EXIT_FAILURE;
        }
        //subscription message
        std::string subscription_message = create_subscription_message("eth trades");

        // Send the subscription message
        ws.write(boost::asio::buffer(subscription_message));
        // Receive messages
        for (;;) {
            boost::beast::multi_buffer buffer;
            ws.read(buffer);
            auto message = boost::beast::buffers_to_string(buffer.data());
            //ws.pong("pong");
            if (message == "{\"result\":null,\"id\":1}") {
                std::cout << "Start Time:\n" << std::put_time(&tm, "%c") << std::endl;
            } else {
                //testing.addCandlestick(message);
                testvolume.addTrade(message);
            }
        }

        std::cout << "End Time:\n" << std::put_time(&tm, "%c") << std::endl;
        return 0;
    }
    catch (std::exception const& e)
    {
        std::time_t t = std::time(nullptr);
        std::tm tm = *std::localtime(&t);
        std::cerr << "Stop Time:\n" << std::put_time(&tm, "%c") <<
                    " Error from try: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}



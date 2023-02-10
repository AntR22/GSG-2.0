
#include <chrono>
#include "responseConstructor.hpp"
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
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
#include "data.hpp"
#include "indicators.hpp"

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

void liveTest (cData &cS, timeProfile &tP, volumeProfile &vP) {
        auto apiKey = "33d6fUXxSpFAeQDQ77yYMpx8Qhlop4zCj7k16En3uvkjKQnwM61EFbhUMkLcXkwr";
        auto secretKey = "Y1UBX9a38Gk38Zpx4eWigQReqahJKyIFGjpUzlqrv6PVZvPFKZGAxLSJQWnuJpB0";
        std::string host = "testnet.binance.vision";
        std::string port = "443";
        
        cData candlesticks(ONEMIN_ONEWEEK);
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

        ws.handshake(host, "/ws-api/v3", ec);

        if(ec) {
            std::cerr << "Error with handshake: " << ec.message() << std::endl;
            return;
        }
       
        ws.write(boost::asio::buffer(tradeRequestObj(apiKey, "BUY", "MARKET", "1.1", secretKey)));
        boost::beast::multi_buffer buffer;
        ws.read(buffer);
        std::cout << boost::beast::make_printable(buffer.data()) << std::endl; 
        
    return;
}
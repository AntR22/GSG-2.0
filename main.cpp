#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/beast/core/ostream.hpp>
#include <string>
#include <utility>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <iostream>
#include "JSONParser.hpp"
#include "data.hpp"


#define ONEHOUR_ONEMONTH 672
#define ONEMIN_ONEWEEK 10080
#define ONESEC_ONEDAY 86400

namespace json = boost::json;

std::string create_subscription_message() {
    std::ifstream file("details.json");
    value message = parse(file);
    std::string s = serialize(message);
    return s;
}

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp; 

int main() {
    try {
        cData candlesticks(ONEMIN_ONEWEEK);
        std::string s = "{\n  \"e\": \"kline\",\n  \"E\": 123456789,\n  \"s\": \"ETHUSDT\",\n  \"k\": {\n    \"t\": 123400000,\n    \"T\": 123460000,\n    \"s\": \"BNBBTC\",\n    \"i\": \"1m\",\n    \"f\": 100,\n    \"L\": 200,\n    \"o\": \"0.0010\",\n    \"c\": \"0.0020\",\n    \"h\": \"0.0025\",\n    \"l\": \"0.0015\",\n    \"v\": \"1000\",\n    \"n\": 100,\n    \"x\": false,\n    \"q\": \"1.0000\",\n    \"V\": \"500\",\n    \"Q\": \"0.500\",\n    \"B\": \"123456\"\n  }\n}";
        candlesticks.addCandlestick(s);
       // candlesticks.printCandlestick(candlesticks.accessDataAtIndex(0));
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

        ws.handshake(host, "/ws/ethusdt@kline_5m", ec);

        if(ec) {
            std::cerr << "Error with handshake: " << ec.message() << std::endl;
            return EXIT_FAILURE;
        }
        //subscription message
        std::string subscription_message = create_subscription_message();

        // Send the subscription message
        ws.write(boost::asio::buffer(subscription_message));

        // Receive messages
        for (;;) {
            boost::beast::multi_buffer buffer;
            ws.read(buffer);
            std::cout << boost::beast::make_printable(buffer.data()) << std::endl;
            if (buffer.size() == 0) {
                break;
            }
            auto message = boost::beast::buffers_to_string(buffer.data());
            if (message == "ping") {
                buffer.consume(buffer.size());
                ws.write(boost::asio::buffer("pong"));
            }
        }

    }
    catch (std::exception const& e)
    {
        std::cerr << "Error from try: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
}

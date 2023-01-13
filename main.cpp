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
#include <iostream>
#include "JSONParser.hpp"
#include "data.hpp"

#define ONEHOUR_ONEMONTH 672
#define ONEMIN_ONEWEEK 10080
#define ONESEC_ONEDAY 86400

std::string create_subscription_message() {
    boost::property_tree::ptree message;
    message.put("method", "SUBSCRIBE");

    std::vector<std::string> streams = {"btcusdt@kline_1m"};
    boost::property_tree::ptree params;
    for(auto& stream: streams)
        params.push_back(std::make_pair("", boost::property_tree::ptree(stream)));

    message.add_child("params", params);
    message.put("id", 1);

    std::stringstream ss;
    boost::property_tree::write_json(ss, message);
    return ss.str();
}

int main() {
    try {
        cData candlesticks(ONEMIN_ONEWEEK);
        std::string s = "{\n  \"e\": \"kline\",\n  \"E\": 123456789,\n  \"s\": \"BNBBTC\",\n  \"k\": {\n    \"t\": 123400000,\n    \"T\": 123460000,\n    \"s\": \"BNBBTC\",\n    \"i\": \"1m\",\n    \"f\": 100,\n    \"L\": 200,\n    \"o\": \"0.0010\",\n    \"c\": \"0.0020\",\n    \"h\": \"0.0025\",\n    \"l\": \"0.0015\",\n    \"v\": \"1000\",\n    \"n\": 100,\n    \"x\": false,\n    \"q\": \"1.0000\",\n    \"V\": \"500\",\n    \"Q\": \"0.500\",\n    \"B\": \"123456\"\n  }\n}";
        candlesticks.addCandlestick(s);
        candlesticks.printCandlestick(candlesticks.accessDataAtIndex(0));
        // WebSocket endpoint
        std::string host = "wss://stream.binance.com";
        std::string port = "443";

        // Create the I/O context
        boost::asio::io_context ioc;
        
        // Create the WebSocket stream
        boost::beast::websocket::stream<boost::asio::ip::tcp::socket> ws(ioc);

        // Resolve the hostname
        boost::asio::ip::tcp::resolver resolver(ioc);
        auto endpoints = resolver.resolve(host, port);

        // Connect to the first endpoint in the list
        boost::asio::connect(ws.next_layer(), endpoints);

        boost::beast::error_code ec;
        std::cout << "here1" << std::endl;

        ws.handshake(host, "/ws/btcusdt@kline_1m", ec);
        std::cout << "here2" << std::endl;

        if(ec) {
            std::cerr << "Error: " << ec.message() << std::endl;
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
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
}

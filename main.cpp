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
/*
std::string create_subscription_message() {
    boost::property_tree::ptree message;
    message.put("method", "SUBSCRIBE");

    boost::property_tree::ptree params;
    params.put("", "btcusdt@kline_1m");
    message.add_child("params", params);
    message.put("id", 1);

    std::stringstream ss;
    boost::property_tree::write_json(ss, message);
    return ss.str();
}
*/

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
// WebSocket endpoint
        std::string host = "wss://data-stream.binance.com";
        std::string port = "9443";

        // Create the I/O context
        boost::asio::io_context ioc;

        boost::beast::websocket::stream<boost::asio::ip::tcp::socket> ws(ioc);
                std::cout << "here" << std::endl;

        boost::beast::error_code ec1;
        boost::asio::connect(ws.next_layer(), boost::asio::ip::tcp::resolver(ioc).resolve(host, port,ec1));
        std::cerr << "Error: " << ec1 << ", " << ec1.message() << std::endl;

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

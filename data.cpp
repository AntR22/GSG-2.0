#include "data.hpp"

using namespace boost::json;

tradeData setTradeData (std::string s) {
    value const &obj = parse(s);

    double price = std::stod(value_to<std::string>(obj.at("p")));
    double vol = std::stod(value_to<std::string>(obj.at("q")));
    int64_t timei = value_to<int64_t>(obj.at("T"));
    int time = timei;
    tradeData tD(price, vol, time);

    return tD;
}

std::string checkDataInterval (std::string s) {
    error_code ec;
    value const &objJS = parse(s, ec);
    if (ec) {
        std::cout << "Parse Failed:" << ec.message() << std::endl;
    }
    value const &obj = objJS.at("k");
    return value_to<std::string>(obj.at("i"));
}

candlestick createCandlestickObject (std::string s) {
    error_code ec;
    value const &objJS = parse(s, ec);
    if (ec) {
        std::cout << "Parse Failed:" << ec.message() << std::endl;
    }
    value const &obj = objJS.at("k");
    double oP = std::stod(value_to<std::string>(obj.at("o")));
    double cP = std::stod(value_to<std::string>(obj.at("c")));
    double pH = std::stod(value_to<std::string>(obj.at("h")));
    double pL = std::stod(value_to<std::string>(obj.at("l")));
    double bV = std::stod(value_to<std::string>(obj.at("v")));
    double qV = std::stod(value_to<std::string>(obj.at("q")));
    bool closed = value_to<bool>(obj.at("x"));
    candlestick c (oP, cP, pH, pL, bV, qV, closed);

    return c;
}

std::string create_subscription_message(std::string stream) {
    value message;
    if (stream == "eth trades") {
        message = {
            {"method", "SUBSCRIBE"},
            {"params", {"ethusdt@aggTrade"}},
            {"id", 1}
        };
    } else if (stream == "eth klines") {
        message = {
            {"method", "SUBSCRIBE"},
            {"params", {"ethusdt@kline_1m"}},
            {"id", 1}
        };
    }
    return serialize(message);
}
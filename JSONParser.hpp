#pragma once

#define BOOST_JSON_STACK_BUFFER_SIZE 1024
#include <iostream>
#include <sstream>
#include <string>
#include <boost/json/src.hpp>

class candlestick{
    private:
        double openPrice;
        double closePrice;
        double priceHigh;
        double priceLow;
        double baseVolume;
        double quoteVolume;
        bool isClosed;

    public:
        candlestick (double oP = 0, double cP = 0, double pH = 0, double pL = 0, double bV = 0, double qV = 0, bool iC = false) {
        openPrice = oP;
        closePrice = cP;
        priceHigh = pH;
        priceLow = pL;
        baseVolume = bV;
        quoteVolume = qV;
        isClosed = iC;
        };

        void setopenPrice(double p) {
            openPrice = p;
        }
        double getopenPrice(void) {
            return openPrice;
        }
        void setclosePrice(double p) {
            closePrice = p;
        }
        double getclosePrice(void) {
            return closePrice;
        }
        void setpriceHigh(double p) {
            priceHigh = p;
        }
        double getpriceHigh(void) {
            return priceHigh;
        }
        void setpriceLow(double p) {
            priceLow = p;
        }
        double getpriceLow(void) {
            return priceLow;
        } 
        void setbaseVolume(double v) {
            baseVolume = v;
        }
        double getbaseVolume(void) {
            return baseVolume;
        }
        void setquoteVolume(double v) {
            quoteVolume = v;
        }
        double getquoteVolume(void) {
            return quoteVolume;
        } 
        void setClosed(bool c) {
            isClosed = c;
        }
        bool getClosed(void) {
            return isClosed;
        }
};

using namespace boost::json;

inline std::string checkDataInterval (std::string s) {
    error_code ec;
    value const &objJS = parse(s, ec);
    if (ec) {
        std::cout << "Parse Failed:" << ec.message() << std::endl;
    }
    value const &obj = objJS.at("k");
    return value_to<std::string>(obj.at("i"));
}

inline candlestick createCandlestickObject (std::string s) {
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

inline std::string create_subscription_message() {
    value message = {
        {"method", "SUBSCRIBE"},
        {"params", {"ethusdt@kline_1m"}},
        {"id", 1}
    };
    return serialize(message);
}

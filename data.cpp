#include "data.hpp"
#include "responseConstructor.hpp"
#include <boost/json/src.hpp>

using namespace boost::json;

tradeData setTradeData (std::string s) {
    value const &obj = parse(s);

    double price = std::stod(value_to<std::string>(obj.at("p")));
    double vol = std::stod(value_to<std::string>(obj.at("q")));
    int64_t timei = value_to<int64_t>(obj.at("t"));
    tradeData tD(price, vol, timei);

    return tD;
}

timeCandlestick createTimeCandle (std::string s) {
    value const &objJS = parse(s);

    value const &obj = objJS.at("k");
    double oP = std::stod(value_to<std::string>(obj.at("o")));
    double cP = std::stod(value_to<std::string>(obj.at("c")));
    double pH = std::stod(value_to<std::string>(obj.at("h")));
    double pL = std::stod(value_to<std::string>(obj.at("l")));
    int64_t timei = value_to<int64_t>(obj.at("t"));
    bool closed = value_to<bool>(obj.at("x"));
    timeCandlestick c (timei, oP, cP, pH, pL, closed);

    return c;
}

std::string checkDataInterval (std::string s) {
    value const &objJS = parse(s);

    value const &obj = objJS.at("k");
    return value_to<std::string>(obj.at("i"));
}

candlestick createCandlestickObject (std::string s) {
    value const &objJS = parse(s);

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
    } else if (stream == "eth klines 1m") {
        message = {
            {"method", "SUBSCRIBE"},
            {"params", {"ethusdt@kline_1m"}},
            {"id", 1}
        };
    } else if (stream == "eth klines 1s") {
        message = {
            {"method", "SUBSCRIBE"},
            {"params", {"ethusdt@kline_1s"}},
            {"id", 1}
        };
    } else {
        std::cout << "no sub msg" << std::endl;
    }
    return serialize(message);
}


class paramBuild {
        private:
                boost::json::object params = {};
                std::map<std::string, std::string> paramsList{};
        public:
                void apiKey(std::string key) {
                        params.emplace("apiKey", key);
                        paramsList.emplace("apiKey", key);
                }
                void symbolsArr(std::string token) {
                        boost::json::array arr({token});
                        params.emplace("symbols", arr);
                }
                void limit(int num) {
                        params.emplace("limit", num);
                }
                void fromId(int id) {
                        params.emplace("fromId", id);
                }
                void symbolTrade(std::string symbol) {
                        params.emplace("symbol", symbol);
                        paramsList.emplace("symbol", symbol);
                }
                void time(void) {
                        const auto t1 = std::chrono::system_clock::now();
                        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(t1.time_since_epoch()).count();
                        params.emplace("timestamp", time);
                        std::stringstream strm;
                        strm << time;
                        paramsList.emplace("timestamp", strm.str());
                }
                void side(std::string orderType) {
                        params.emplace("side", orderType);
                        paramsList.emplace("side", orderType);
                }
                void type(std::string order) {
                        params.emplace("type", order);
                        paramsList.emplace("type", order);
                }
                void price(std::string priced) {
                        params.emplace("price", priced);
                        paramsList.emplace("price", priced);
                }
                void quantity(std::string quant) {
                        params.emplace("quantity", quant);
                        paramsList.emplace("quantity", quant);
                }
                
                void signature(std::string secretKey) {
                        params.emplace("signature", hashQuery(paramsList, secretKey.c_str()));
                } 
                boost::json::object objectComp() {
                        return params;
                }
};

std::string queryString(std::map<std::string, std::string> &parameters) {
        std::string urlQuery = "";
        for (auto it = parameters.cbegin(); it != parameters.cend(); ++it) {
                if (it == parameters.cbegin()) {
                        urlQuery += it->first + '=' + it->second; 
                } else {
                        urlQuery += '&' + it->first + '=' + it->second;
                }
        }
        return urlQuery;
}

std::string hashQuery(std::map<std::string, std::string> &parameters, const char* key) {
        std::string query = queryString(parameters);
        const char* data = query.c_str();
        unsigned char *result;
        static char res_hexstring[64];
        int result_len = 32;
        std::string signature;

        result = HMAC(EVP_sha256(), key, strlen((char *)key), const_cast<unsigned char *>(reinterpret_cast<const unsigned char*>(data)), strlen((char *)data), NULL, NULL);
  	for (int i = 0; i < result_len; i++) {
    	        sprintf(&(res_hexstring[i * 2]), "%02x", result[i]);
  	}

  	for (int i = 0; i < 64; i++) {
  		signature += res_hexstring[i];
  	}

  	return signature;
}

namespace json = boost::json;

std::string tradeRequestObj(std::string apiKey, std::string side, std::string type, std::string quantity, std::string secret) {
        paramBuild params;
        params.apiKey(apiKey);
        params.side(side);
        params.symbolTrade("ETHUSDT");
        params.time();
        params.type(type);
        params.quantity(quantity);
        params.signature(secret);
        boost::json::object trade({
            {"id", std::to_string(rand())},
            {"method", "order.place"},
            {"params", params.objectComp()}
        });
        return serialize(trade);
}   

#pragma once

#include <boost/json/src.hpp>
#include <map>
#include <openssl/hmac.h>
#include <cstdio>

inline std::string queryString(std::map<std::string, std::string> &parameters) {
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

inline std::string hashQuery(std::map<std::string, std::string> &parameters, const char* key) {
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
class paramBuild {
        private:
                object params = {};
                std::map<std::string, std::string> paramsList{};
        public:
                void apiKey(std::string key) {
                        params.emplace("apiKey", key);
                        paramsList.emplace("apiKey", key);
                }
                void symbolsArr(std::string token) {
                        array arr({token});
                        std::cout << arr << std::endl;
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
                object objectComp() {
                        return params;
                }
};

inline std::string tradeRequestObj(std::string apiKey, std::string side, std::string type, std::string quantity, std::string secret) {
        paramBuild params;
        params.apiKey(apiKey);
        params.side(side);
        params.symbolTrade("ETHUSDT");
        params.time();
        params.type(type);
        params.quantity(quantity);
        params.signature(secret);
        object trade({
            {"id", std::to_string(rand())},
            {"method", "order.place"},
            {"params", params.objectComp()}
        });
        return serialize(trade);
}   


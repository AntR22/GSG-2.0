#include <openssl/hmac.h>
#include <unordered_map>
#include <cstdio>
#include <map>

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
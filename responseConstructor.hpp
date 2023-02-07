#pragma once

#include "data.hpp"
#include <map>
#include <openssl/hmac.h>
#include <cstdio>

std::string queryString(std::map<std::string, std::string> &parameters);

std::string hashQuery(std::map<std::string, std::string> &parameters, const char* key);

std::string tradeRequestObj(std::string apiKey, std::string side, std::string type, std::string quantity, std::string secret);

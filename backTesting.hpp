#pragma once

#include <string>
#include <filesystem>
#include "data.hpp"

candlestick fillCandleStick(std::string line);

void parseCSV(std::string &directory, cData &cS, std::mutex &m, int &runKey);

void backTest (cData &cS, timeProfile &tP, volumeProfile &vP, std::string directory, std::mutex &m, int &runKey);
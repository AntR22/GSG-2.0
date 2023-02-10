#pragma once

#include <string>
#include "data.hpp"

candlestick fillCandleStick(std::string line);

void parseCSV(std::string &directory, cData &cS);

void backTest (cData &cS, timeProfile &tP, volumeProfile &vP);
#pragma once

#include <string>
#include "data.hpp"

void fillCandleStick(std::string line, candlestick &data);

void parseCSV(std::string &directory);

void backTest (cData &cS, timeProfile &tP, volumeProfile &vP);
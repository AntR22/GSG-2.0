#pragma once

#include "data.hpp"
#include "JSONParser.hpp"
#include <string>
#include <iostream>

#define FAILURE -1

inline double basicIndicators(std::string type, cData candlesticks, int period = 0) {
    if (period == 0 || period > candlesticks.returnSize()) {
        period = candlesticks.returnSize();
    }

    if (type == "VWAP") {
        double sum = 0;
        double totalVolume = 0;
        double volume;
        for (int i = 0; i < period; i++) {
            volume = candlesticks.accessDataAtTime(i).getquoteVolume();
            totalVolume += volume;
            sum += (volume / 4) * (candlesticks.accessDataAtTime(i).getopenPrice() +
                   candlesticks.accessDataAtTime(i).getclosePrice() +
                   candlesticks.accessDataAtTime(i).getpriceHigh() + 
                   candlesticks.accessDataAtTime(i).getpriceLow());
        }
        return sum / totalVolume;

    } else if (type == "SMA") {
        double sum = 0;
        for (int i = 0; i < period; i++) {
            sum += candlesticks.accessDataAtTime(i).getopenPrice() +
                   candlesticks.accessDataAtTime(i).getclosePrice() +
                   candlesticks.accessDataAtTime(i).getpriceHigh() + 
                   candlesticks.accessDataAtTime(i).getpriceLow() / 4;
        }
        return sum / period;

    } else if (type == "RSI") {
        double AG = 0;
        double AL = 0;
        for (int i = 1; i < period; i++) {
            if (candlesticks.accessDataAtTime(i - 1).getclosePrice() >=
                candlesticks.accessDataAtTime(i).getclosePrice()) {
                AG += (candlesticks.accessDataAtTime(i - 1).getclosePrice() -
                       candlesticks.accessDataAtTime(i).getclosePrice()) / 
                       candlesticks.accessDataAtTime(i - 1).getclosePrice();
            } else {
                AL += ((candlesticks.accessDataAtTime(i).getclosePrice() -
                       candlesticks.accessDataAtTime(i - 1).getclosePrice()) / 
                       candlesticks.accessDataAtTime(i).getclosePrice());
            }
        }

        AG = AG / period;
        AL = AL / period;
        if (AL == 0) {
            return 100;
        }
        double RS = (AG / AL);

        return (100 - (100 / (1 + RS)));

    } else {
        std::cout << "Incorrect type, Usage:\n-SMA\n-RSI\n-VWAP" << std::endl;
        return FAILURE;
    }
}
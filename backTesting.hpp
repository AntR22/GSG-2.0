#pragma once

#include <chrono>
#include "data.hpp"
#include <fstream>
#include <filesystem>


#define ONEHOUR_ONEMONTH 672
#define ONEMIN_ONEWEEK 10080
#define ONESEC_ONEDAY 86400

inline void fillCandleStick(std::string line, candlestick &data) {
    std::stringstream candle(line);
    std::string component;
    auto allValues = std::vector<std::string>{};
    for (int i = 0; i < 8; i++) {
        getline(candle, component, ',');
        allValues.push_back(component);
    }
    data.setopenPrice(stod(allValues[1]));
    data.setpriceHigh(stod(allValues[2]));
    data.setpriceLow(stod(allValues[3]));
    data.setclosePrice(stod(allValues[4]));
    data.setbaseVolume(stod(allValues[5]));
    data.setquoteVolume(stod(allValues[7]));
    data.setClosed(true);
}

namespace fs = std::filesystem;
inline void parseCSV(std::string &directory) {
    for (const auto &entry : fs::directory_iterator(directory)) {
        if (!entry.is_directory()) {
            continue;
        }
        int numFiles = 0;
        for (const auto &file : fs::directory_iterator(entry)) {
            numFiles++;
        }
        cData newTest(10);
        for (auto i = 1; i <= numFiles; i++) {
            std::string path = entry.path().generic_string() + "/" + std::to_string(i) + ".csv";
            std::ifstream fin(path);
            std::string line;
            int j = 0;
            while (getline(fin, line)) {
                std::cout << "line" << std::endl;
                candlestick data;
                fillCandleStick(line, data);
                newTest.addCandlestick(data);
            }
            newTest.printAllData();
        }
    }
}

inline int backTest () {
    std::string directory = "../historicalData";
    parseCSV(directory);
    return EXIT_SUCCESS;
}


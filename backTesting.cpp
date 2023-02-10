#include "data.hpp"
#include "backTesting.hpp"
#include <fstream>
#include <chrono>
#include <filesystem>
#include<thread>

#define ONEHOUR_ONEMONTH 672
#define ONEMIN_ONEWEEK 10080
#define ONESEC_ONEDAY 86400

candlestick fillCandleStick(std::string line) {
    candlestick data;
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

    return data;
}

namespace fs = std::filesystem;

void parseCSV(std::string &directory, cData &cS) {
    for (const auto &entry : fs::directory_iterator(directory)) {
        if (!entry.is_directory()) {
            continue;
        }
        int numFiles = 0;
        for (const auto &file : fs::directory_iterator(entry)) {
            numFiles++;
        }
        for (auto i = 1; i <= numFiles; i++) {
            std::string path = entry.path().generic_string() + "/" + std::to_string(i) + ".csv";
            std::ifstream fin(path);
            std::string line;
            int j = 0;
            while (getline(fin, line)) {
                cS.addCandlestick(fillCandleStick(line));
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            cS.printAllData();
        }
    }
}

void backTest (cData &cS, timeProfile &tP, volumeProfile &vP) {
    std::string directory = "../historicalData/ETH-1h/7-11.11.22(1h)";
    parseCSV(directory, cS);
    return;
}


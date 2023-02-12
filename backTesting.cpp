#include "data.hpp"
#include "backTesting.hpp"
#include <fstream>
#include <chrono>
#include <filesystem>
#include <thread>

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

void parseCSV(const std::string &directory, cData &cS, std::mutex &m,  int &runKey) {
    // Get the list of all .csv files in the directory
    std::vector<fs::path> files;
    for (const auto &entry : fs::directory_iterator(directory)) {
        if (entry.path().extension() == ".csv") {
            files.push_back(entry.path());
        }
    }

    // Sort the list of files in order
    std::sort(files.begin(), files.end());
    int j = 0;

    // Process each .csv file in order
    for (const auto &file : files) {
        std::ifstream fin(file.generic_string());
        std::string line;
        while (getline(fin, line)) {
            cS.addCandlestick(fillCandleStick(line));
            if (j == 0 && cS.allCandlesClosed()) {
                j++;
                m.unlock();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
        }
    }
    // termiates other thread after all info has been passed in
    std::this_thread::sleep_for(std::chrono::milliseconds(25));
    runKey = 1;
}


void backTest (cData &cS, timeProfile &tP, volumeProfile &vP, std::string directory, std::mutex &m, int &runKey) {
    m.lock();
    fs::path DIR = "../" + directory;
    parseCSV(DIR.string(), cS, m, runKey);
    return;
}


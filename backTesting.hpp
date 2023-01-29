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
    
    getline(candle, component, ',');
    
    getline(candle, component, ',');
    data.setopenPrice(stod(component));
    getline(candle, component, ',');
    data.setpriceHigh(stod(component));
   
    getline(candle, component, ',');
    data.setpriceLow(stod(component));
    
    getline(candle, component, ',');
    data.setclosePrice(stod(component));
    
    getline(candle, component, ',');
    data.setbaseVolume(stod(component));
    
    getline(candle, component, ',');
  
    getline(candle, component, ',');
    data.setquoteVolume(stod(component));
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
        cData newTest(ONEMIN_ONEWEEK);
        for (auto i = 1; i <= numFiles; i++) {
            std::string path = entry.path().generic_string() + "/" + std::to_string(i) + ".csv";
            std::ifstream fin(path);
            std::string line;
            while(getline(fin, line)) {
                candlestick data;
                fillCandleStick(line, data);
                newTest.addCandlestick(data);
            }
            
        }
        
    }
}

inline int backTest () {
    std::string directory = "../historicalData";
    parseCSV(directory);
    return EXIT_SUCCESS;
}


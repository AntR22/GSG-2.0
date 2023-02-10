#include "liveTrades.hpp"
#include "backTesting.hpp"
#include "liveTesting.hpp"
#include "processes.hpp"
#include "strategies.hpp"
#include <thread>
#include <iostream>
#include <utility>

#define DEFAULT_PRECISION 0.1

int main(int argc, char *argv[]) {
    if (argcErrorMessage(argc)) {
        return -1;
    }

    arguments args (argc, argv);
    cData candlestickData (args.dataPoints);
    timeProfile timeData (DEFAULT_PRECISION, args.profileSize);
    volumeProfile volumeData (DEFAULT_PRECISION, args.profileSize, true, 0.01);
    if (args.runSetting == 1) {
        // initialise objs and pass in reference to objects into threads
        std::thread dataCollection(marketStream, std::ref(candlestickData), std::ref(timeData), std::ref(volumeData));
        std::thread strategy(VWAP, std::ref(candlestickData), std::ref(timeData), std::ref(volumeData));
        // need a way of handling when datacollection throws an exception
        dataCollection.join();
        strategy.join();
    } else if (args.runSetting == 2) {
        liveTest(candlestickData, timeData, volumeData);
    } else if (args.runSetting == 3) {
        backTest(candlestickData, timeData, volumeData);
    }

    return 0;
}


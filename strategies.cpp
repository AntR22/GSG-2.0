#include "strategies.hpp"
#include "indicators.hpp"
#include <thread>

// this is an example of how it should be set out for backtesting
// must enter while loop relativity early in code or else timing between threads is off and lock
// will be unlocked too early
void VWAP1 (cData &cS, timeProfile &tP, volumeProfile &vP, std::mutex &m, int &runKey) {
    double VWAPValue;
    int counter = -1;
    while (true) {
        if (cS.returnStart() != counter) {
            m.lock();
            counter = cS.returnStart();
            VWAPValue = basicIndicators("VWAP", cS, 10);
            m.unlock();
        }
        // sleep for shorter if large workload is performed during if statement
        // test with high precision clock
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (runKey == 1) {
            return;
        }
    }
}

void VWAP (cData &cS, timeProfile &tP, volumeProfile &vP) {
    double VWAPValue;
    int counter = cS.returnStart();
    while (true) {
        if (true) {
            VWAPValue = basicIndicators("VWAP", cS, 10);
            std::cout << VWAPValue << std::endl;
            return;
        }
    }
}
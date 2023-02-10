#include "strategies.hpp"
#include "indicators.hpp"

void VWAP (cData &cS, timeProfile &tP, volumeProfile &vP) {
    double VWAPValue;
    cS.returnSize();
    while (true) {
        if (cS.allCandlesClosed()) {
            VWAPValue = basicIndicators("VWAP", cS, 10);
            if (VWAPValue < cS.accessDataAtTime(1).getclosePrice()){
                std::cout << "cS" << std::endl;
                cS.printCandlestick(1);
            }
        }
    }
}
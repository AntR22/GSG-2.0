#include "strategies.hpp"
#include "indicators.hpp"

void VWAP (cData &cS, timeProfile &tP, volumeProfile &vP) {
    double VWAPValue;

    while (true) {
        if (cS.allCandlesClosed()) {
            VWAPValue = basicIndicators("VWAP", cS);
            if (VWAPValue < cS.accessDataAtTime(1).getclosePrice()){
                std::cout << "buy" << std::endl;
                cS.printCandlestick(1);
                cS.printCandlestick(0);
            }
        }
    }
}
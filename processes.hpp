#pragma once
#include <stdio.h>
#include <cmath>

bool argcErrorMessage (int argc);

int checkRunSetting (char *argone);

int checkSymbol (char *argtwo);

int checkTimePeriod (char *argthree);

int checkDataTime (char *argFour);

int checkStrategy (char *argFive);

int calculateDataPoints (int period, int dataTime);

int64_t checkProfileSize (char *argSeven);

class arguments {
    public:
        int runSetting;
        int symbol;
        int timePeriod;
        int dataTime;
        int strategyUsed;
        int dataPoints;
        int64_t profileSize = 0;

        arguments (int argc, char *argv[]) {
            runSetting = checkRunSetting(argv[1]);
            symbol = checkSymbol(argv[2]);
            timePeriod = checkTimePeriod(argv[3]);
            dataTime = checkDataTime(argv[4]);
            strategyUsed = checkStrategy(argv[5]);
            dataPoints = calculateDataPoints(timePeriod, dataTime);
            if (argc == 7) {
                profileSize = checkProfileSize(argv[6]);
            }
        }
};
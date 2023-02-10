#include "processes.hpp"
#include <iostream>
#include <ctime>
#include <chrono>

#define ONEHOUR_ONEMONTH 672
#define ONEMIN_ONEWEEK 10080
#define ONESEC_ONEHOUR 3600
#define FIVEMIN_ONEHOUR 12
#define ONEMIN_ONEHOUR 60

bool argcErrorMessage (int argc, char* argone) {
    if (checkRunSetting(argone) == 3) {
        if (argc < 7) {
            std::cerr << "Invalid Input\nUsage: GSG-2.0 backtest symbol time-period data-run-time strategy data-directory" << std::endl;
            return true;
        }
    } else if (argc < 6) {
        std::cerr << "Invalid Input\nUsage: GSG-2.0 runSetting symbol time-period data-run-time strategy profile-size" << std::endl;
        return true;
    }
    return false;
}

int checkRunSetting (char *argone) {
    if (strcmp(argone, "livetrade") == 0) {
        return 1;
    } else if (strcmp(argone, "livetest") == 0) {
        return 2;
    } else if (strcmp(argone, "backtest") == 0) {
        return 3;
    } else {
        std::cerr << "Argument 1 Incorrect, Usage:\n-livetrade\n-livetest\n-backtest" << std::endl;
        return 0;
    }
}

int checkSymbol (char *argtwo) {
    if (strcmp(argtwo, "ETH") == 0) {
        return 1;
    } else if (strcmp(argtwo, "BTC") == 0) {
        return 2;
    } else if (strcmp(argtwo, "SOL") == 0) {
        return 3;
    }  else {
        std::cerr << "Argument 2 Incorrect, Usage:\n-ETH\n-BTC\n-SOL" << std::endl;
        return 0;
    }
}

int checkTimePeriod (char *argthree) {
    if (strcmp(argthree, "1s") == 0) {
        return 1;
    } else if (strcmp(argthree, "1m") == 0) {
        return 2;
    } else if (strcmp(argthree, "5m") == 0) {
        return 3;
    } else if (strcmp(argthree, "1h") == 0) {
        return 4;
    } else {
        std::cerr << "Argument 3 Incorrect, Usage:\n-1s\n-1m\n-5m\n-1h" << std::endl;
        return 0;
    }
}

int checkDataTime (char *argFour) {
    if (strcmp(argFour, "1h") == 0) {
        return 1;
    } else if (strcmp(argFour, "1d") == 0) {
        return 2;
    } else if (strcmp(argFour, "2d") == 0) {
        return 3;
    } else if (strcmp(argFour, "1w") == 0) {
        return 4;
    } else if (strcmp(argFour, "1mo") == 0) {
        return 5;
    } else {
        std::cerr << "Argument 4 Incorrect, Usage:\n-1h\n-1d\n-2d\n-1w\n-1mo" << std::endl;
        return 0;
    }
}

int checkStrategy (char *argFive) {
    if (strcmp(argFive, "VWAP") == 0) {
        return 1;
    } else if (strcmp(argFive, "-") == 0) {
        return 2;
    } else if (strcmp(argFive, "-") == 0) {
        return 3;
    } else if (strcmp(argFive, "-") == 0) {
        return 4;
    } else if (strcmp(argFive, "-") == 0) {
        return 5;
    } else {
        std::cerr << "Argument 4 Incorrect, Usage:\n-VWAP\n-\n-\n-\n-" << std::endl;
        return 0;
    }
}

int64_t checkProfileSize (char *argSix) {
    if (strcmp(argSix, "1h") == 0) {
        return (int64_t)3600;
    } else if (strcmp(argSix, "1d") == 0) {
        return (int64_t)86400;
    } else {
        std::cerr << "Argument 7 Incorrect, Usage:\n-1h\n-1d\n-\n-\n-" << std::endl;
        return 0;
    }
}

int calculateDataPoints (int period, int dataTime) {
    int dataPoints = 1;
    if (dataTime == 1 && period == 4) {
        return 1;
    } else if (period <= 2) {
        dataPoints *= pow(60, 3 - period);
    } else if (period == 3) {
        dataPoints *= FIVEMIN_ONEHOUR;
    }
    if (dataTime == 1) {
        return dataPoints;
    }
    dataPoints *= 24;
    if (dataTime == 2) {
        return dataPoints;
    } else if (dataTime == 3) {
        return (dataPoints * 2);
    } else if (dataTime == 4) {
        return (dataPoints * 7);
    } else if (dataTime == 5) {
        return (dataPoints * 30);
    }
    return dataPoints;
}

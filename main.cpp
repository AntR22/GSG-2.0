#include "liveTrades.hpp"
#include "backTesting.hpp"

int main(int argc, char **argv) {
    bool testing = true;
    if (testing) {
        backTest();
    } else {
        marketStream();
    }
    return 0;
}
#include "liveTrades.hpp"
#include "backTesting.hpp"

int main(int argc, char **argv) {
    bool testing = false;
    if (testing) {
        backTest();
    } else {
        marketStream();
    }
    return 0;
}
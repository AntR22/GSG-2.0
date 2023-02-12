#pragma once
#include "data.hpp"

void VWAP1 (cData &cS, timeProfile &tP, volumeProfile &vP, std::mutex &m, int &runKey);

void VWAP (cData &cS, timeProfile &tP, volumeProfile &vP);
#include "pch.h"
#include "RoboTime.h"

std::chrono::time_point<std::chrono::high_resolution_clock> RoboTime::GetNowTime() {
    const std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
    return now;
}

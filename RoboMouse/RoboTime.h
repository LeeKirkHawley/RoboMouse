#pragma once
#include <chrono>

#ifdef COMPILING_DLL
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif

class DLLEXPORT RoboTime
{
public:
    std::chrono::time_point<std::chrono::high_resolution_clock> GetNowTime();
};


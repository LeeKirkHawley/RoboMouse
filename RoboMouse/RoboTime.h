#pragma once
#include <chrono>
#include <string>

//#ifdef COMPILING_DLL
//#define DLLEXPORT __declspec(dllexport)
//#else
//#define DLLEXPORT __declspec(dllimport)
//#endif

class RoboTime
{
public:
    static std::wstring GetNowTime();
};


#pragma once
#include <chrono>
#include <string>

class RoboTime
{
public:
    static std::wstring GetNowTime();
    static std::chrono::milliseconds GetNowTimestamp();
    static std::wstring milliseconds_to_wstring(std::chrono::milliseconds ms);
    static std::chrono::milliseconds wstring_to_milliseconds(const std::wstring& wstr);
};


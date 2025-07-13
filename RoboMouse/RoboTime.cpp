#include <iostream>  
#include <iomanip>  
#include <chrono>  
#include <ctime>  
#include <sstream>
#include <string>
#include <stdexcept>
#include "pch.h"  
#include "RoboTime.h"  


std::wstring RoboTime::GetNowTime() {  

    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm;
    localtime_s(&now_tm, &now_time_t);

    // Get milliseconds
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch());
    std::wstring msStr = std::to_wstring(ms.count());
    std::wstring outputString = L"milliseconds: " + msStr + L"\n";
    //OutputDebugStringW(outputString.c_str());

    std::wstringstream wss;
    wss << std::put_time(&now_tm, L"%Y-%m-%d %H:%M:%S");
    wss << L'.' << std::setfill(L'0') << std::setw(3) << ms.count();
    std::wstring timeStr = wss.str();


   return timeStr;
}

std::chrono::milliseconds RoboTime::GetNowTimestamp() {

    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);

    // Get milliseconds
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch());

    return ms;
}

std::wstring RoboTime::milliseconds_to_wstring(std::chrono::milliseconds ms) 
{
    return std::to_wstring(ms.count());
}

std::chrono::milliseconds RoboTime::wstring_to_milliseconds(const std::wstring& wstr)
{
    try {
        // Convert wstring to long
        wchar_t* end;
        long long value = std::wcstoll(wstr.c_str(), &end, 10);

        // Check if the entire string was consumed and no invalid characters were found
        if (*end != L'\0') {
            throw std::invalid_argument("Invalid characters in wstring");
        }

        // Ensure the value is non-negative (if applicable to your use case)
        if (value < 0) {
            throw std::invalid_argument("Negative milliseconds not allowed");
        }

        return std::chrono::milliseconds(value);
    }
    catch (const std::exception& e) {
        throw std::invalid_argument("Failed to convert wstring to milliseconds: " + std::string(e.what()));
    }
}

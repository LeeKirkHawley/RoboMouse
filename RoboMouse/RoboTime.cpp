#include "pch.h"  
#include "RoboTime.h"  
#include <iostream>  
#include <iomanip>  
#include <chrono>  
#include <ctime>  
#include <sstream> // allows stringstream initialization  

std::wstring RoboTime::GetNowTime() {  
   auto now = std::chrono::system_clock::now();  
   auto now_c = std::chrono::system_clock::to_time_t(now);  

   std::tm now_tm;  
   localtime_s(&now_tm, &now_c); // Use localtime_s correctly  

   std::stringstream ss;  
   ss << std::put_time(&now_tm, "%Y-%m-%d %X");  
   std::string time_str = ss.str();  
   const std::wstring ws(time_str.begin(), time_str.end());  

   return ws;  
}

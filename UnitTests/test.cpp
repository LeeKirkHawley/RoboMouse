#include "pch.h"
#include <chrono>
#include "../RoboMouse/RoboTime.h"

#ifdef COMPILING_DLL
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif


TEST(RoboTimeTests, GetNowTime) {

    RoboTime roboTime;
    //roboTime.GetNowTime();
  //EXPECT_EQ(1, 1);
  //EXPECT_TRUE(true);
}
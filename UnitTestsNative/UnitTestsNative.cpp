#include "pch.h"
#include "CppUnitTest.h"
#include "../RoboMouse/RoboTime.h"
#include "pch.h"
#include <chrono>
#include <thread>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestsNative
{
	TEST_CLASS(UnitTestsNative)
	{
	public:
		
		TEST_METHOD(GetNowtimeTest)
		{
			std::wstring time = RoboTime::GetNowTime();
			Assert::IsNotNull(&time, L"time should not be null");
		}

		TEST_METHOD(GetNowTimestampTest)
		{
			std::chrono::milliseconds time = RoboTime::GetNowTimestamp();
			Assert::IsNotNull(&time, L"time should not be null");
		}

		TEST_METHOD(CompareTimestampsTest)
		{
			std::chrono::milliseconds time1 = RoboTime::GetNowTimestamp();
			Assert::IsNotNull(&time1, L"time should not be null");

			std::this_thread::sleep_for(std::chrono::seconds(1));

			std::chrono::milliseconds time2 = RoboTime::GetNowTimestamp();
			Assert::IsNotNull(&time2, L"time should not be null");

            Assert::AreNotEqual(time1.count(), time2.count(), L"Timestamps should not be equal");
		}
	};
}

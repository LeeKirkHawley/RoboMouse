#include "pch.h"
#include "CppUnitTest.h"
#include "../RoboMouse/RoboTime.h"
#include "pch.h"
#include <chrono>


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
	};
}

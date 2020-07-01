#pragma once
#include "framework.h"
#include <string>

using namespace std;

class RoboMouseEvent
{

public:
	wstring Event;

	RoboMouseEvent(int eventId, int X, int Y) {
		
		switch (eventId)
		{
		case WM_LBUTTONDOWN:
			Event.assign(L"LeftButtonDown");
			Event.append(L",");
			Event.append(to_wstring(X));
			Event.append(L",");
			Event.append(to_wstring(Y));
			break;

		case WM_LBUTTONUP:
			Event.assign(L"LeftButtonUp");
			Event.append(L",");
			Event.append(to_wstring(X));
			Event.append(L",");
			Event.append(to_wstring(Y));
			break;

		case WM_MOUSEMOVE:
			Event.assign(L"MouseMove");
			Event.append(L",");
			Event.append(to_wstring(X));
			Event.append(L",");
			Event.append(to_wstring(Y));
			break;

		default:

			break;
		}
	};
};

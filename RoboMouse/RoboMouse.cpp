// RoboMouse.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "RoboMouse.h"
#include <iostream>
#include <vector>
#include <windowsx.h>
#include <sstream>
#include <fstream>
#include <string>
#include "resource.h"
#include "RoboMouseEvent.h"

using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HHOOK mouseHook;
bool hookActive = false;
std::vector<wstring> vecEvents;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void                SetHook();
void                ReleaseHook();
void                WriteEvents();
void                ReplayFile();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ROBOMOUSE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ROBOMOUSE));
    
    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ROBOMOUSE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ROBOMOUSE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      300, 200, 300, 200, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


LRESULT __stdcall MouseHookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        switch (wParam)
        {
        case WM_LBUTTONDOWN: 
        {
            if (hookActive == true) {
                POINT cursorPoint;
                GetCursorPos(&cursorPoint);

                wstring Event = L"";
                Event.assign(L"LeftButtonDown");
                Event.append(L",");
                Event.append(to_wstring(cursorPoint.x));
                Event.append(L",");
                Event.append(to_wstring(cursorPoint.y));

                vecEvents.push_back(Event);
            }

            // ACTIVE WINDOW
            //HWND clickedWindow = WindowFromPoint(cursorPoint);
            //wchar_t wnd_title[256];
            //GetWindowText(clickedWindow, wnd_title, sizeof(wnd_title));
            //wstringstream windowInfo;
            //windowInfo << L"Window: " << clickedWindow << " " << wnd_title << L"\n";
            //OutputDebugString(windowInfo.str().c_str());

            
        }
        break;

        case WM_LBUTTONUP:
        {
            POINT cursorPoint;
            GetCursorPos(&cursorPoint);

            wstring Event = L"";
            Event.assign(L"LeftButtonUp");
            Event.append(L",");
            Event.append(to_wstring(cursorPoint.x));
            Event.append(L",");
            Event.append(to_wstring(cursorPoint.y));

            vecEvents.push_back(Event);
        }
        break;

        case WM_MOUSEMOVE: 
        {
            POINT cursorPoint;
            GetCursorPos(&cursorPoint);

            wstring Event = L"";
            Event.assign(L"MouseMove");
            Event.append(L",");
            Event.append(to_wstring(cursorPoint.x));
            Event.append(L",");
            Event.append(to_wstring(cursorPoint.y));

            vecEvents.push_back(Event);
        }

        default:
            //cout << "Some other mouse event" << endl;
            //OutputDebugString(L"Some other mouse event\n");
            break;
        }
    }
    return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case ID_GO:
                if (hookActive == false) {
                    SetHook();
                    hookActive = true;
                }
                else {
                    ReleaseHook();
                    hookActive = false;
                    WriteEvents();
                }
                break;
            case ID_FILE_REPLAY:
                ReplayFile();
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void SetHook()
{
    if (!(mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookCallback, NULL, 0)))
        //if (!(mouseHook = SetWindowsHookEx(WH_MOUSE, MouseHookCallback, NULL, 0)))
    {
        cout << "Failed to install mouse hook!" << endl;
    }
}

void ReleaseHook()
{
    UnhookWindowsHookEx(mouseHook);
}

void WriteEvents() 
{
    wofstream file1;
    file1.open("c:\\\\temp\\RoboMouseEvents.txt");

    if (!file1.is_open())
    {
        throw new exception("Couldn't open file.");
    }

    for (vector<wstring>::const_iterator i = vecEvents.begin(); i != vecEvents.end(); ++i) {
        char str[80];

        file1 << *i;
        file1 << '\n';
    }

    file1.close();
}

void ReplayFile()
{
    wifstream file1;
    
    file1.open("c:\\\\temp\\RoboMouseEvents.txt");
    if (!file1.is_open())
    {
        throw new exception("Couldn't open file.");
    }

    vecEvents.clear();

    std::wstring line;
    while (std::getline(file1, line))
    {
        vecEvents.push_back(line);
    }

    file1.close();
}
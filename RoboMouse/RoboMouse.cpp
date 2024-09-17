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
#include "..\KLib\KLib.h"
#include <windows.h>
#include <shlobj.h>
#include <string_view>
#include <stdio.h>
#include <wchar.h>
#include "FilePath.h"
#include <iostream>

using namespace std;

#define MAX_LOADSTRING 100

HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HHOOK mouseHook;
bool hookActive = false;
std::vector<wstring> vecEvents;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void                SetHook();
void                ReleaseHook();
void                WriteEvents();
void                ReplayFile();
bool                DoesFileExist(LPWSTR lpszFilename);
long counter = 0;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ROBOMOUSE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ROBOMOUSE));
    
    MSG msg;

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

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; 

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
            counter++;
            
            if (hookActive == true && counter % 5)
            {
                POINT cursorPoint;
                GetCursorPos(&cursorPoint);

                wstring Event = L"";
                Event.assign(L"MouseMove");
                Event.append(L",");
                Event.append(to_wstring(cursorPoint.x));
                Event.append(L",");
                Event.append(to_wstring(cursorPoint.y));

                //SYSTEMTIME st;
                //GetSystemTime(&st);
                //OutputDebugString(st.wMilliseconds);

                vecEvents.push_back(Event);
            }
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
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case ID_GO:
                counter = 0;
                SetHook();
                hookActive = true;
                break;
            case ID_STOP:
                hookActive = false;
                ReleaseHook();
                WriteEvents();
                break;
            case ID_FILE_REPLAY: 
                {
                    ReplayFile();
                }

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
    FilePath filePath;
    std::wstring path = filePath.getPath(L"RoboMouseEvents.txt");

    if(DoesFileExist((LPWSTR)path.c_str()))
        DeleteFile(path.c_str());

    wofstream file1;
    file1.open(path.c_str());

    if (!file1.is_open())
    {
        throw new exception("Couldn't open file.");
    }

    for (vector<wstring>::const_iterator i = vecEvents.begin(); i != vecEvents.end(); ++i) 
    {
        file1 << *i;
        file1 << '\n';
    }

    file1.close();
}

void ReplayFile()
{
    wifstream file1;
    FilePath filePath;
    std::wstring path = filePath.getPath(L"RoboMouseEvents.txt");
    
    file1.open(path.c_str());
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

    for (vector<wstring>::const_iterator i = vecEvents.begin(); i != vecEvents.end(); ++i) {

        vector<wstring> splits = KLib::Split(*i);

        if (splits[0] == L"MouseMove") {
            SetCursorPos(stoi(splits[1]), stoi(splits[2]));
            Sleep(10);
        }
        else if (splits[0] == L"LeftButtonDown") {
            INPUT Inputs[1] = { 0 };

            Inputs[0].type = INPUT_MOUSE;
            Inputs[0].mi.dx = stoi(splits[1]); // desired X coordinate
            Inputs[0].mi.dy = stoi(splits[2]); // desired Y coordinate
            Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;

            SendInput(1, Inputs, sizeof(INPUT));
        }
        else if (splits[0] == L"LeftButtonUp") {
            INPUT Inputs[1] = { 0 };

            Inputs[0].type = INPUT_MOUSE;
            Inputs[0].mi.dx = stoi(splits[1]); // desired X coordinate
            Inputs[0].mi.dy = stoi(splits[2]); // desired Y coordinate
            Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;

            SendInput(1, Inputs, sizeof(INPUT));
        }
    }

    file1.close();
}

bool DoesFileExist(LPWSTR lpszFilename)
{
    DWORD attr = GetFileAttributes(lpszFilename);

    if (attr == INVALID_FILE_ATTRIBUTES)
        return false;
    else
        return true;
}


#pragma once
#pragma once
#include <vector>
#include <string>


#ifdef KLIB_EXPORTS 
#define KLIB_API  __declspec(dllexport) 
#else 
#define KLIB_API  __declspec(dllimport) 
#endif 

using namespace std;


#define BREAK_ON_FAIL(value)            if(FAILED(value)) break;
#define BREAK_ON_NULL(value, newHr)     if(value == NULL) { hr = newHr; break; }


template <class T> void SafeRelease(T** ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}



class KLib
{
private:

public:
    KLIB_API static wstring GetLastErrorMessage(DWORD LastError);
    KLIB_API static vector<wstring> Split(wstring Incoming);
    KLIB_API static vector<string> Split(string Incoming);
    KLIB_API static std::string string_format(const std::string& fmt, ...);
};

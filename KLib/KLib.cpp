#include "pch.h"
#include "KLib.h"

using namespace std;

wstring KLib::GetLastErrorMessage(DWORD LastError)
{
	TCHAR errmsg[512];

	if (!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, LastError, 0, errmsg, 511, NULL))
		return L"GetLastErrorMessage() failed.";

	return errmsg;
}

vector<wstring> KLib::Split(wstring Incoming)
{
	vector<wstring> Tokens;
	wchar_t* buffer;
	wchar_t* part = wcstok_s(&Incoming[0], L",", &buffer);
	while (part)
	{
		Tokens.push_back(part);
		part = wcstok_s(NULL, L",", &buffer);
	}

	return Tokens;
}

vector<string> KLib::Split(string Incoming)
{
	vector<string> Tokens;
	char* buffer;
	char* part = strtok_s(&Incoming[0], ",", &buffer);
	while (part)
	{
		Tokens.push_back(string(part));
		part = strtok_s(NULL, ",", &buffer);
	}

	return Tokens;
}


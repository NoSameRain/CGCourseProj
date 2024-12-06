#include"Utils.h"

void debugLog(const std::string& message) {
	OutputDebugStringA((message + "\n").c_str());
}

void debugLog(const std::wstring& message) {
	OutputDebugString((message + L"\n").c_str());
}
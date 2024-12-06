#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <Windows.h>  // Required for OutputDebugString
#include <string>

void debugLog(const std::string& message);

void debugLog(const std::wstring& message);
#endif // UTILS_H
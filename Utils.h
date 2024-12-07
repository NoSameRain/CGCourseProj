#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <Windows.h>  // Required for OutputDebugString
#include <string>
#include <fstream>

void debugLog(const std::string& message);

void debugLog(const std::wstring& message);

void writeStringToFile(std::ofstream& outfile, std::string& filePath);

void readStringfromFile(std::ifstream& infile, std::string& filePath);

#endif // UTILS_H


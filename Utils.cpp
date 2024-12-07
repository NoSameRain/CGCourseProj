#include"Utils.h"

void debugLog(const std::string& message) {
	OutputDebugStringA((message + "\n").c_str());
}

void debugLog(const std::wstring& message) {
	OutputDebugString((message + L"\n").c_str());
}
void writeStringToFile(std::ofstream& outfile, std::string& filePath) {
	size_t length = filePath.size();
	outfile.write(reinterpret_cast<const char*>(&length), sizeof(size_t));
	outfile.write(filePath.c_str(), length);
}

void readStringfromFile(std::ifstream& infile, std::string& filePath) {
	size_t length;
	infile.read(reinterpret_cast<char*>(&length), sizeof(length));
	filePath.resize(length);
	infile.read(&filePath[0], length);
}
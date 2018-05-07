#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>


class XMLfileReader
{
public:
	XMLfileReader();
	XMLfileReader(const char* File_Path);
	int Load(const char* File_Path, bool Full_headers = true, bool debugMode = false);
	std::vector<std::string> XMLData;
	std::vector<std::string> XMLHeaders;
};

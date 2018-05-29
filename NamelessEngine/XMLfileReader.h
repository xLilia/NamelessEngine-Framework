#pragma once
#include "NamelessCore.hpp"
#include <vector>
#include <set>
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>


class XMLfileReader
{
public:
	XMLfileReader();
	XMLfileReader(const char* File_Path);
	int Load(const char* File_Path, bool debugMode = false);
	void RESET();

	std::vector<std::string> XMLData;
	
	void GetColladaData();
	_NL::Core::MeshInstanceData MeshData;

};

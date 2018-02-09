#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "NamelessCore.h"

class OBJfileReader
{
public:
	OBJfileReader();
	void RESET();
	int LOAD(const char* OBJpath);
	_NL::Core::MeshData Mshd;
	std::vector<_NL::Core::vIndices> Is;
	~OBJfileReader();
};


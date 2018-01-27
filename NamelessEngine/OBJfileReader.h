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
	void LOAD(const char* OBJpath);
	std::vector<_NL::Core::Vertex> verts;
	std::vector<GLushort> indices;
	~OBJfileReader();
};


#pragma once
#include "NamelessCore.h"
#include <vector>
class OBJfileReader
{
public:
	OBJfileReader();
	~OBJfileReader();

	int LOAD(const char* OBJpath);
	_NL::Core::MeshData Mshd;
	std::vector<_NL::Core::vIndices> Is;
	GLint MTL_counter = -1;
	void RESET();

};


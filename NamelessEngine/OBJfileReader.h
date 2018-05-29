#pragma once
#include "NamelessCore.hpp"
#include <vector>
class OBJfileReader
{
public:
	OBJfileReader();
	~OBJfileReader();

	int LOAD(const char* OBJpath);
	_NL::Core::MeshInstanceData Mshd;
	//std::vector<_NL::Core::vIndices> Is;
	//GLint MTL_counter = -1;
	void RESET();

};


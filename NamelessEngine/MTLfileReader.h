#pragma once
#include "NamelessCore.h"
#include "TextureLoader.h"
#include <vector>
class MTLfileReader
{
public:
	MTLfileReader();
	~MTLfileReader();

	int LOAD(const char* OBJpath);
	std::vector<_NL::Core::MTLdata>MTLD;
	GLint inMTLfile_counter = -1;
	TextureLoader TL;
	void RESET();
};


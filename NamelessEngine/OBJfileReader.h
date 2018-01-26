#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <glm\glm.hpp>

#include "NamelessCore.h"

class OBJfileReader
{
public:
	OBJfileReader();
	void newShapeGL();
	void makeSimpleShapeGL(const char* OBJpath, glm::vec3 color);
	std::vector<_NLess::Vertex> verts;
	std::vector<GLushort> indices;
	
	~OBJfileReader();
};


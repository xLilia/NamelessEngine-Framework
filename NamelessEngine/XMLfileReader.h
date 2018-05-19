#pragma once
#include "NamelessCore.hpp"
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
	
	void GetColladaData();
	std::vector<std::vector<glm::vec3>>mesh_positions_array;
	std::vector<std::vector<glm::vec3>>mesh_normals_array;
	std::vector<std::vector<glm::vec2>>mesh_map_array;
	std::vector<std::vector<glm::vec3>>mesh_triangles_array;

};

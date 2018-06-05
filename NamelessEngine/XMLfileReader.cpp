#include "XMLfileReader.h"
#include "XMLfileReader.h"

_NL::Tools::XMLfileReader::XMLfileReader()
{
}

_NL::Tools::XMLfileReader::XMLfileReader(const const char* File_Path)
{
	Load(File_Path);
}

void _NL::Tools::XMLfileReader::GetColladaData(){
	for (GLuint n = 0; n < XMLData.size(); n++) {
		
		GLuint n1;
		size_t G = XMLData[n].find("<geometry id=");
		size_t A = XMLData[n].find("<animation id=");
		size_t C = XMLData[n].find("<controller id =");
		size_t V = XMLData[n].find("<visual_scene id="); 


		if (G != -1) {
			for (n1 = n; n < XMLData.size(); n1++) {
				size_t END = XMLData[n1].find("</geometry>");

				size_t fArray = XMLData[n1].find("<float_array id=");

				size_t TriArray = XMLData[n1].find("<p>");

				if (END != -1) break;

				if (fArray != -1 || TriArray != -1) {

					size_t fA_P = XMLData[n1].find("-positions-array");
					size_t fA_N = XMLData[n1].find("-normals-array");
					size_t fA_M = XMLData[n1].find("-map-array");

					size_t start = XMLData[n1].find('>');
					size_t end = XMLData[n1].find('</');
					std::string PosS = XMLData[n1].substr(start + 1, end - start - 1);
					int xyz = 0;
					std::string Num;
					glm::vec3 newV;
					for (char c : PosS) {
						if (c == ' ' || c == '<') {
							if (fA_M == -1) {
								switch (xyz)
								{
								case 0: newV.x = std::stof(Num); break;
								case 1: newV.y = std::stof(Num); break;
								case 2: newV.z = std::stof(Num); 
									xyz = -1;
									if (fA_P != -1) MeshData.mesh_positions_array.push_back(newV);
									if (fA_N != -1) MeshData.mesh_normals_array.push_back(newV);
									if (TriArray != -1)
										MeshData.mesh_triangles_array.push_back(newV);
								}
							}
							else {
								switch (xyz)
								{
								case 0: newV.s = std::stof(Num); break;
								case 1: newV.t = std::stof(Num);
									xyz = -1;
									if (fA_M != -1) MeshData.mesh_map_array.push_back(newV);
								}
							}
							xyz = xyz++;
							Num.clear();
						}
						else {
							Num += c;
						}
					}
				}
			}
			n = n1;
		}

		if (A != -1) {

		}

		if (C != -1) {

		}

		if (V != -1) {

		}
	}
	////MESH OPTIMIZATION
	//for (size_t i = 0; i < Vmesh_positions_array.size(); i++)
	//	for (size_t j = 0; j < MeshData.mesh_positions_array.size(); j++)
	//	{
	//		glm::vec3::bool_type EQ = glm::equal(Vmesh_positions_array[i], MeshData.mesh_positions_array[j]);
	//		
	//	}
	//return;
}

int _NL::Tools::XMLfileReader::Load(const char* File_Path, bool debugMode) {
	

	std::ifstream f(File_Path);
	std::string strf;

	f.seekg(0, std::ios::out);
	strf.reserve(f.tellg());
	f.seekg(0, std::ios::beg);

	strf.assign(
		std::istreambuf_iterator<char>(f),
		std::istreambuf_iterator<char>()
	);

	f.close();

	size_t DataSnipBEG = 0;
	size_t DataSnipOUT = -1;

	for (unsigned int cPos = 0; cPos < strf.length(); cPos++)
	{
		if (strf[cPos] == '\n') {
			DataSnipOUT = cPos;

			std::string D = strf.substr(DataSnipBEG, DataSnipOUT - DataSnipBEG);

			if (D[0] == '\n') {
				D = "NULL_DATA";
			}

			if (debugMode) std::cout << D << std::endl << std::endl;

			XMLData.push_back(D);

			DataSnipBEG = DataSnipOUT+1;
			DataSnipOUT = -1;
		}
	}
	return 0;
}

void _NL::Tools::XMLfileReader::RESET()
{
	MeshData.mesh_map_array.clear();
	MeshData.mesh_normals_array.clear();
	MeshData.mesh_positions_array.clear();
	MeshData.mesh_triangles_array.clear();
	XMLData.clear();
}

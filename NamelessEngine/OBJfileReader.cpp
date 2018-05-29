#include "OBJfileReader.h"

OBJfileReader::OBJfileReader()
{

}

void OBJfileReader::RESET()
{
	//this->Is.clear();
	//this->Is.shrink_to_fit();
	
	this->Mshd.mesh_positions_array.clear();
	
	this->Mshd.mesh_normals_array.clear();
	
	this->Mshd.mesh_map_array.clear();

	this->Mshd.mesh_triangles_array.clear();

	//this->MTL_counter = -1;
}

int OBJfileReader::LOAD(const char * OBJpath)
{
	FILE * file = std::fopen(OBJpath,"r");
	if (!file) {
		std::cout << "ERROR: FAILED TO LOAD: "<< OBJpath << std::endl;
		return -1;
	}
	for (;;) {
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		
		if(res == EOF)
			break;
		
		////Material
		if (std::strcmp(lineHeader, "v") == 0) {
			glm::vec3 V;
			fscanf(file, "%f %f %f\n", 
				&V.x, 
				&V.y, 
				&V.z
			);
			this->Mshd.mesh_positions_array.push_back(V);
		}//Indices
		else if(std::strcmp(lineHeader, "f") == 0) {
			glm::vec3 tri1;
			glm::vec3 tri2;
			glm::vec3 tri3;

			fscanf(file, "%f/%f/%f %f/%f/%f %f/%f/%f\n",
				&tri1.x, //position
				&tri1.z, //normal
				&tri1.y, //texCoord
				&tri2.x,
				&tri2.z,
				&tri2.y,
				&tri3.x,
				&tri3.z,
				&tri3.y
			);
			
			tri1.x -= 1;
			tri1.y -= 1;
			tri1.z -= 1;
			tri2.x -= 1;
			tri2.y -= 1;
			tri2.z -= 1;
			tri3.x -= 1;
			tri3.y -= 1;
			tri3.z -= 1;

			//tri.MTL_ID = MTL_counter;
			this->Mshd.mesh_triangles_array.push_back(tri1);
			this->Mshd.mesh_triangles_array.push_back(tri2);
			this->Mshd.mesh_triangles_array.push_back(tri3);
		}//Vertex TextureCoordinates
		else if (std::strcmp(lineHeader, "vt") == 0) {
			glm::vec2 Vmap;
			fscanf(file, "%f %f\n", 
				&Vmap.s,
				&Vmap.t
			);
			this->Mshd.mesh_map_array.push_back(Vmap);
		}//Vertex Normal
		else if (std::strcmp(lineHeader, "vn") == 0) {
			glm::vec3 VNorm;
			fscanf(file, "%f %f %f\n",
				&VNorm.x,
				&VNorm.y,
				&VNorm.z
			);
			this->Mshd.mesh_normals_array.push_back(VNorm);
		}
		//else if (std::strcmp(lineHeader, "usemtl") == 0) {
		//	MTL_counter++;
		//}
	}
	std::fclose(file);
	return 0;
}

OBJfileReader::~OBJfileReader()
{
}



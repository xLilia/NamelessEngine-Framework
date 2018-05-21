#include "OBJfileReader.h"

OBJfileReader::OBJfileReader()
{

}

void OBJfileReader::RESET()
{
	//this->Is.clear();
	//this->Is.shrink_to_fit();
	//
	//this->Mshd.vPos.clear();
	//this->Mshd.vPos.shrink_to_fit();
	//
	//this->Mshd.vCol.clear();
	//this->Mshd.vCol.shrink_to_fit();
	//
	//this->Mshd.vNorm.clear();
	//this->Mshd.vNorm.shrink_to_fit();
	//
	//this->Mshd.vTexC.clear();
	//this->Mshd.vTexC.shrink_to_fit();
	//
	//this->MTL_counter = -1;
}

int OBJfileReader::LOAD(const char * OBJpath)
{
	//FILE * file = std::fopen(OBJpath,"r");
	//if (!file) {
	//	std::cout << "ERROR: FAILED TO LOAD: "<< OBJpath << std::endl;
	//	return -1;
	//}
	//for (;;) {
	//	char lineHeader[128];
	//	int res = fscanf(file, "%s", lineHeader);
	//	
	//	if(res == EOF)
	//		break;
	//	
	//	////Material
	//	if (std::strcmp(lineHeader, "v") == 0) {
	//		_NL::Core::VertexPos v;
	//		fscanf(file, "%f %f %f\n", 
	//			&v.Pos.x, 
	//			&v.Pos.y, 
	//			&v.Pos.z
	//		);
	//		this->Mshd.vPos.push_back(v);
	//	}//Indices
	//	else if(std::strcmp(lineHeader, "f") == 0) {
	//		_NL::Core::vIndices tri;
	//
	//		fscanf(file, "%i/%i/%i %i/%i/%i %i/%i/%i\n",
	//			&tri.v[0],
	//			&tri.vt[0],
	//			&tri.vn[0],
	//			&tri.v[1],
	//			&tri.vt[1],
	//			&tri.vn[1],
	//			&tri.v[2],
	//			&tri.vt[2],
	//			&tri.vn[2]
	//		);
	//
	//		tri.MTL_ID = MTL_counter;
	//		this->Is.push_back(tri);
	//	}//Vertex TextureCoordinates
	//	else if (std::strcmp(lineHeader, "vt") == 0) {
	//		_NL::Core::VertexTexCoord vt;
	//		fscanf(file, "%f %f\n", 
	//			&vt.TexCoord.s,
	//			&vt.TexCoord.t
	//		);
	//		this->Mshd.vTexC.push_back(vt);
	//	}//Vertex Normal
	//	else if (std::strcmp(lineHeader, "vn") == 0) {
	//		_NL::Core::VertexNorm vn;
	//		fscanf(file, "%f %f %f\n",
	//			&vn.Norm.x,
	//			&vn.Norm.y,
	//			&vn.Norm.z
	//		);
	//		this->Mshd.vNorm.push_back(vn);
	//	}
	//	else if (std::strcmp(lineHeader, "usemtl") == 0) {
	//		MTL_counter++;
	//	}
	//}
	//std::fclose(file);
	return 0;
}

OBJfileReader::~OBJfileReader()
{
}



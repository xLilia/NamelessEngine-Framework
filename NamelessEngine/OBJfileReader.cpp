#include "OBJfileReader.h"

OBJfileReader::OBJfileReader()
{
}

void OBJfileReader::RESET()
{
	this->verts.clear();
	this->indices.clear();
}

int OBJfileReader::LOAD(const char * OBJpath)
{
	FILE * file = std::fopen(OBJpath,"r");
	if (!file) {
		std::cout << "ERROR -3: FAILED TO LOAD: "<< OBJpath << std::endl;
		return -1;
	}
	for (;;) {
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		
		if(res == EOF)
			break;
		
		if (std::strcmp(lineHeader, "v") == 0) {
			_NL::Core::Vertex v;
			fscanf(file, "%f %f %f\n", &v.P.x, &v.P.y, &v.P.z);
			this->verts.push_back(v);
		}else

		if (std::strcmp(lineHeader, "f") == 0) {
			int a, b, c;
			fscanf(file, "%i %i %i\n", &a, &b, &c);
			this->indices.push_back(a);
			this->indices.push_back(b);
			this->indices.push_back(c);
		}
	}
	std::fclose(file);
}

OBJfileReader::~OBJfileReader()
{
}

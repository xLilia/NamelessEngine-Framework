#include "OBJfileReader.h"

OBJfileReader::OBJfileReader()
{
	//this->color = glm::vec4(0.0, 1.0, 1.0, 1.0);
}

void OBJfileReader::newShapeGL()
{
	this->verts.clear();
	this->indices.clear();
	//sthis->color = glm::vec4(1.0,1.0,1.0,1.0);
}

void OBJfileReader::makeSimpleShapeGL(const char * OBJpath, glm::vec3 color)
{
	FILE * file = std::fopen(OBJpath,"r");
	if (!file) {
		std::cout << "FAILED TO LOAD: "<< OBJpath << std::endl;
		int a;
		std::cin >> a;
		exit(1);
	}
	for (;;) {
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		
		if(res == EOF)
			break;
		
		if (std::strcmp(lineHeader, "v") == 0) {
			_NLess::Vertex v;
			fscanf(file, "%f %f %f\n", &v.Pos.x, &v.Pos.y, &v.Pos.z);
			v.color = color;
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

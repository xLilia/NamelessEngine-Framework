#include "MeshObj.h"

_NL::Element::MeshObj::MeshObj()
{
}

_NL::Element::MeshObj::MeshObj(const char * filePath)
{
	LoadOBJFile(filePath);
}

int _NL::Element::MeshObj::LoadOBJFile(const char * filePath)
{
	if (OBJF.LOAD(filePath) != 0) {
		return -1;
	}
	MeshData = OBJF.Mshd;
	Indices = OBJF.Is;
	OBJF.RESET();
	return 0;
}

std::string _NL::Element::MeshObj::ClassName() const
{
	return "_NL::Element::MeshObj";
}

_NL::Element::MeshObj::~MeshObj()
{
}

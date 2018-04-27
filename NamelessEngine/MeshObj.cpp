#include "MeshInstance.h"

_NL::Element::MeshInstance::MeshInstance()
{
}

_NL::Element::MeshInstance::MeshInstance(const char * filePath)
{
	LoadOBJFile(filePath);
}

int _NL::Element::MeshInstance::LoadOBJFile(const char * filePath)
{
	if (OBJF.LOAD(filePath) != 0) {
		return -1;
	}
	MeshData = OBJF.Mshd;
	Indices = OBJF.Is;
	OBJF.RESET();
	return 0;
}

char* _NL::Element::MeshInstance::ClassName() const
{
	return "_NL::Element::MeshInstance";
}

_NL::Element::MeshInstance::~MeshInstance()
{
}

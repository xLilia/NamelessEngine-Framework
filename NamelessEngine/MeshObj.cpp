#include "MeshInstance.h"

_NL::Element::MeshInstance::MeshInstance()
{
}

_NL::Element::MeshInstance::MeshInstance(const char * filePath)
{
	size_t Typefound = -1;
	Typefound = std::string(filePath).find(".obj", 0);
	if (Typefound != -1) {
		LoadOBJFile(filePath);
	}
	else {
		Typefound = std::string(filePath).find(".dae", 0);
		if (Typefound != -1) {
			LoadColladaFile(filePath);
		}
	}
}

int _NL::Element::MeshInstance::LoadColladaFile(const char * filePath)
{
	if (ColladaF.Load(filePath,true,true) != 0) {
		return -1;
	}

	ColladaF.GetColladaData();

	return 0;
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

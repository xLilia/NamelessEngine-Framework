#include "MeshRenderer.h"

_NL::Component::MeshRenderer::MeshRenderer()
{
}

std::string _NL::Component::MeshRenderer::ClassName() const
{
	return "_NL::Component::MeshRenderer";
}

_NL::Component::MeshRenderer::~MeshRenderer()
{
}

int _NL::Component::MeshRenderer::LoadOBJFile(const char * filePath)
{
	OBJF.LOAD(filePath);
	return 0;
}

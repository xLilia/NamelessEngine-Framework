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
	if (OBJF.LOAD(filePath) != 0) {
		return -1;
	}
	Mesh.verts = OBJF.verts.data();
	Mesh.indices = OBJF.indices.data();
	OBJF.RESET();
	return 0;
}

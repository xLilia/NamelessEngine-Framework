#include "ShaderObj.h"

_NL::Element::ShaderObj::ShaderObj(std::string name)
{
	this->name = name;
}

void _NL::Element::ShaderObj::InstallShader(const char * VertexShader, const char * FragmentShader)
{
	Shader = new ShaderWizard();
	Shader->installShaders(VertexShader, FragmentShader);
}

std::string _NL::Element::ShaderObj::ClassName() const
{
	return "_NL::Element::ShaderObj";
}

_NL::Element::ShaderObj::ShaderObj::~ShaderObj()
{
}
#include "ShaderObj.h"

_NL::Element::ShaderObj::ShaderObj()
{
}

_NL::Element::ShaderObj::ShaderObj(const char * VertexShader, const char * FragmentShader)
{
	InstallShader(VertexShader, FragmentShader);
}

void _NL::Element::ShaderObj::InstallShader(const char * VertexShader, const char * FragmentShader)
{
	Shader.clearPrograms();
	Shader.installShaders(VertexShader, FragmentShader);
}

GLuint _NL::Element::ShaderObj::getShaderProgram() const{
	return Shader.InstlledProgramIDs[0];
}

std::string _NL::Element::ShaderObj::ClassName() const
{
	return "_NL::Element::ShaderObj";
}

_NL::Element::ShaderObj::ShaderObj::~ShaderObj()
{
}
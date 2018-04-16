#include "ShaderInstance.h"

_NL::Element::ShaderInstance::ShaderInstance()
{
}

_NL::Element::ShaderInstance::ShaderInstance(const char * VertexShader, const char * FragmentShader)
{
	InstallShader(VertexShader, FragmentShader);
}


void _NL::Element::ShaderInstance::InstallShader(const char * VertexShader, const char * FragmentShader)
{
	Shader.clearPrograms();
	Shader.installShaders(VertexShader, FragmentShader);
}

GLuint _NL::Element::ShaderInstance::getShaderProgram() const{
	return Shader.InstlledProgramIDs[0];
}

void _NL::Element::ShaderInstance::Use()
{
	glUseProgram(Shader.InstlledProgramIDs[0]);
}

void _NL::Element::ShaderInstance::UnUse()
{
	glUseProgram(0);
}

std::string _NL::Element::ShaderInstance::ClassName() const
{
	return "_NL::Element::ShaderInstance";
}

_NL::Element::ShaderInstance::ShaderInstance::~ShaderInstance()
{
}
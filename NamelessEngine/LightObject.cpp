#include "LightObject.h"

_NL::Object::LightObject::LightObject()
{
}

_NL::Object::LightObject::LightObject(char* name)
{
	this->name = name;
}

void _NL::Object::LightObject::GenerateFramebuffer(GLuint Shadow_Width, GLuint Shadow_Height)
{

	//Generate Textures
	glDeleteTextures(1, &ShadowMap);
	glDeleteFramebuffers(1, &Framebuffer);

	glGenTextures(1, &ShadowMap);
	glBindTexture(GL_TEXTURE_2D, ShadowMap);
	glTexImage2D(
		GL_TEXTURE_2D, 
		0,
		GL_DEPTH_COMPONENT,
		Shadow_Width,
		Shadow_Height,
		0, 
		GL_DEPTH_COMPONENT,
		GL_FLOAT, 
		NULL
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	//Generate Framebuffers 

	glGenFramebuffers(1, &Framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ShadowMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


char* _NL::Object::LightObject::getTypeName() 
{
	return "_NL::Object::LightObject";
}

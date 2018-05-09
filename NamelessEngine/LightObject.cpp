#include "LightObject.h"

_NL::Object::LightObject::LightObject(std::string name)
{
	this->name = name;
}

void _NL::Object::LightObject::GenerateFramebuffer()
{

	//Generate Textures

	glGenTextures(1, &ShadowMap);
	glBindTexture(GL_TEXTURE_2D, ShadowMap);
	glTexImage2D(
		GL_TEXTURE_2D, 
		0,
		GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, 
		SHADOW_HEIGHT, 
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

void _NL::Object::LightObject::UpdateFramebuffer(_NL::Engine::GameManager* G)
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);
	glClear(GL_DEPTH_BUFFER_BIT);


	float near_plane = 1.0f, far_plane = 7.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

	glm::mat4 lightView = glm::lookAt(
		LightProperties.lightPosition,
		LightProperties.lightDirection,
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

	G->RenderSceneObjects(LightProperties.lightPosition, lightView, lightProjection, G->DepthPassShader.getShaderProgram());

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

char* _NL::Object::LightObject::ClassName() const
{
	return "_NL::Object::LightObject";
}

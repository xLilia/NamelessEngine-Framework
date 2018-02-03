#include "CameraObj.h"

_NL::Object::CameraObj::CameraObj(std::string name, _NL::Object::WorldSpace* LinkedWorld, GLsizei RenderWindowWidth, GLsizei RenderWindowHeight, GLfloat FOV, GLfloat NearPlane, GLfloat FarPlane, GLsizei RenderWindowX, GLsizei RenderWindowY)
{
	this->FOV = FOV;
	this->NearPlane = NearPlane;
	this->FarPlane = FarPlane;
	this->RenderWindowX = RenderWindowX;
	this->RenderWindowY = RenderWindowY;
	this->LinkedWorld = LinkedWorld;
	updateProjectionMatrix();
}

void _NL::Object::CameraObj::updateProjectionMatrix()
{
	glViewport(RenderWindowX, RenderWindowY, RenderWindowWidth, RenderWindowHeight);
	projectionMatrix = glm::perspective(FOV, (GLfloat)RenderWindowWidth / (GLfloat)RenderWindowHeight, NearPlane, FarPlane);
}

std::string _NL::Object::CameraObj::ClassName() const
{
	return "_NL::Object::CameraObj";
}

_NL::Object::CameraObj::~CameraObj()
{
}
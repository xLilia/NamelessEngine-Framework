#include "CameraObj.h"

_NL::Object::CameraObj::CameraObj(std::string name, GLsizei RenderWindowWidth, GLsizei RenderWindowHeight, GLsizei RenderWindowX, GLsizei RenderWindowY, GLfloat FOV, GLfloat NearPlane, GLfloat FarPlane)
{
	this->name = name;
	this->Settings.FOV = FOV;
	this->Settings.NearPlane = NearPlane;
	this->Settings.FarPlane = FarPlane;
	this->Settings.RenderWindowX = RenderWindowX;
	this->Settings.RenderWindowY = RenderWindowY;
	this->Settings.RenderWindowWidth = RenderWindowWidth;
	this->Settings.RenderWindowHeight = RenderWindowHeight;
	updateCameraSettings();
}

void _NL::Object::CameraObj::updateCameraSettings()
{
	updateCameraViewport();
	updateCameraProjectionMatrix();
}

void _NL::Object::CameraObj::updateCameraProjectionMatrix()
{
	projectionMatrix = glm::perspective(Settings.FOV, (GLfloat)Settings.RenderWindowWidth / (GLfloat)Settings.RenderWindowHeight, Settings.NearPlane, Settings.FarPlane);
}

void _NL::Object::CameraObj::updateCameraViewport()
{
	glViewport(Settings.RenderWindowX, Settings.RenderWindowY, Settings.RenderWindowWidth, Settings.RenderWindowHeight);
}

glm::mat4 _NL::Object::CameraObj::getWorldToViewMatrix() const
{
	return glm::lookAt(Transform.position, Transform.position + Transform.LookAtCenter, Transform.rotation);
}

std::string _NL::Object::CameraObj::ClassName() const
{
	return "_NL::Object::CameraObj";
}


_NL::Object::CameraObj::~CameraObj()
{
}


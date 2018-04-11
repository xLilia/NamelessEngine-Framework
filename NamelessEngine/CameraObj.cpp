#include "CameraObj.h"

_NL::Object::CameraObj::CameraObj(std::string name, GLsizei RenderWindowWidth, GLsizei RenderWindowHeight, GLsizei RenderWindowX, GLsizei RenderWindowY, GLfloat FOV, GLfloat NearPlane, GLfloat FarPlane, GLfloat RenderScaleRatio)
{
	this->name = name;
	this->Settings.FOV = FOV;
	this->Settings.NearPlane = NearPlane;
	this->Settings.FarPlane = FarPlane;
	this->Settings.RenderWindowPos.x = RenderWindowX;
	this->Settings.RenderWindowPos.y = RenderWindowY;
	this->Settings.RenderWindowSize.x = RenderWindowWidth;
	this->Settings.RenderWindowSize.y = RenderWindowHeight;
	this->Settings.RenderScaleRatio = RenderScaleRatio;
	updateCameraSettings();
	updateAudioListenerWithCamTransform();
}

void _NL::Object::CameraObj::updateAudioListenerWithCamTransform()
{
	updateAudioListenerPosition(Transform.position);
	updateAudioListenerDirection(Transform.LookAtCenter);
	updateAudioListenerRotation(Transform.rotation);
}

void _NL::Object::CameraObj::updateAudioListenerPosition(glm::vec3 pos)
{
	AudioListener.setPosition(pos.x, pos.y, pos.z);
}

void _NL::Object::CameraObj::updateAudioListenerDirection(glm::vec3 dir)
{
	AudioListener.setDirection(dir.x, dir.y, dir.z);
}

void _NL::Object::CameraObj::updateAudioListenerRotation(glm::vec3 upVec)
{
	AudioListener.setUpVector(upVec.x, upVec.y, upVec.z);
}

void _NL::Object::CameraObj::updateCameraSettings()
{
	updateCameraViewport();
	updateCameraProjectionMatrix();
}

void _NL::Object::CameraObj::updateCameraProjectionMatrix()
{
	projectionMatrix = glm::perspective(glm::radians(Settings.FOV), (GLfloat)Settings.RenderWindowSize.x / (GLfloat)Settings.RenderWindowSize.y, Settings.NearPlane, Settings.FarPlane);
}

void _NL::Object::CameraObj::updateCameraViewport()
{
	glViewport(Settings.RenderWindowPos.x, Settings.RenderWindowPos.y, Settings.RenderWindowSize.x, Settings.RenderWindowSize.y);
}

glm::mat4 _NL::Object::CameraObj::getWorldToViewMatrix() const
{
	return glm::lookAt(Transform.position, Transform.position + Transform.LookAtCenter, Transform.rotation);
}

glm::mat4 _NL::Object::CameraObj::getViewMatrix() const
{
	return glm::lookAt(glm::vec3(), glm::vec3() + Transform.LookAtCenter, Transform.rotation);
}

std::string _NL::Object::CameraObj::ClassName() const
{
	return "_NL::Object::CameraObj";
}


_NL::Object::CameraObj::~CameraObj()
{
}


#include "Transform.h"

_NL::Component::Transform::Transform()
{
	
}

std::string _NL::Component::Transform::ClassName() const
{
	return "_NL::Component::Transform";
}

_NL::Component::Transform::~Transform()
{
}

glm::vec3 _NL::Component::Transform::eulerAngles()
{
	return glm::eulerAngles(transform.QuaternionRotation);
}

glm::quat _NL::Component::Transform::LookAt(glm::vec3 target, glm::vec3 forwardAxis)
{

	glm::vec3 forward = glm::normalize(target - transform.position); //get new Local Z
	
	glm::vec3 worldForward = forwardAxis; //get World Z
	GLfloat dot = glm::dot(worldForward, forward); //getAngle LocalZ > WorldZ
	GLfloat rotationAngle = glm::acos(dot); //AngleIn degrees
	glm::vec3 rotationAxis = glm::normalize(glm::cross(forward, worldForward)); //localUP

	return glm::toQuat(glm::rotate(glm::mat4(), rotationAngle, -rotationAxis));
	
}

//glm::quat _NL::Component::Transform::QuaternionFromAxisAngle(glm::vec3 ax, GLfloat angle) {
//	
//	ax = glm::normalize(ax);
//	GLfloat qw = cos(angle / 2);
//	GLfloat qx = ax.x * sin(angle / 2);
//	GLfloat qy = ax.y * sin(angle / 2);
//	GLfloat qz = ax.z * sin(angle / 2);
//	return glm::normalize(glm::quat(qw, qx, qy, qz));
//}

glm::quat _NL::Component::Transform::RotateEuler(glm::vec3 eulerAngles)
{
	return transform.QuaternionRotation *= glm::quat(eulerAngles);
}




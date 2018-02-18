#include "Transform.h"

_NL::Component::Transform::Transform()
{
	transform.position = glm::vec3(0, 0, 0);
	transform.EulerRotation = glm::vec3(0, 0, 0);
	transform.MatrixRotation = glm::mat4();
	transform.scale = glm::vec3(1, 1, 1);
}

std::string _NL::Component::Transform::ClassName() const
{
	return "_NL::Component::Transform";
}

void _NL::Component::Transform::Rotate(GLfloat x, GLfloat y, GLfloat z)
{
	Rotate(glm::vec3(x, y, z));
}

void _NL::Component::Transform::Rotate(glm::vec3 EulerRotation)
{
	transform.EulerRotation += EulerRotation;
	glm::mat4 RotX = glm::rotate(glm::mat4(), EulerRotation.x, glm::vec3(1, 0, 0));
	glm::mat4 RotY = glm::rotate(glm::mat4(), EulerRotation.y, glm::vec3(0, 1, 0));
	glm::mat4 RotZ = glm::rotate(glm::mat4(), EulerRotation.z, glm::vec3(0, 0, 1));
	transform.MatrixRotation *= RotZ * RotX * RotY;
}

_NL::Component::Transform::~Transform()
{
}



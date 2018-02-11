#include "Transform.h"

_NL::Component::Transform::Transform()
{
	transform.position = glm::vec3(0, 0, 0);
	transform.rotationAxis = glm::vec3(0, 1, 0);
	transform.rotationAngle = 0;
	transform.scale = glm::vec3(1, 1, 1);
}

std::string _NL::Component::Transform::ClassName() const
{
	return "_NL::Component::Transform";
}

_NL::Component::Transform::~Transform()
{
}
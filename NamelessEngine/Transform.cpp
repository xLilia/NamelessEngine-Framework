#include "Transform.h"

_NL::Component::Transform::Transform()
{
	transform.position = glm::vec3(0, 0, 0);
}

std::string _NL::Component::Transform::ClassName() const
{
	return "_NL::Component::Transform";
}

_NL::Component::Transform::~Transform()
{
}
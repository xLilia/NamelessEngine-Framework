#include "LightObject.h"

_NL::Object::LightObject::LightObject(std::string name)
{
	this->name = name;
}

_NL::Object::LightObject::~LightObject()
{
}

char* _NL::Object::LightObject::ClassName() const
{
	return "_NL::Object::LightObject";
}

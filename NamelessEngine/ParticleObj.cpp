#include "ParticleObj.h"

_NL::Object::ParticleObj::ParticleObj()
{
	this->addComponent(new _NL::Component::Transform);
}

_NL::Object::ParticleObj::~ParticleObj()
{
}

char * _NL::Object::ParticleObj::ClassName() const
{
	return "_NL::Object::ParticleObj";
}

#include "ParticleObj.h"

_NL::Object::ParticleObj::ParticleObj()
{
	this->lifeTime = 1;
	this->Awake = false;
	this->Alive = true;
}

char * _NL::Object::ParticleObj::ClassName() const
{
	return "_NL::Object::ParticleObj";
}

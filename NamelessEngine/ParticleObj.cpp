#include "ParticleObj.h"

_NL::Object::ParticleObj::ParticleObj(_NL::Component::MeshRenderer* PMesh)
{
	this->addComponent(PMesh);
	this->addComponent(new _NL::Component::Transform);
	this->lifeTime = 1;
	this->Awake = false;
	this->Alive = true;
}

_NL::Object::ParticleObj::~ParticleObj()
{
}

char * _NL::Object::ParticleObj::ClassName() const
{
	return "_NL::Object::ParticleObj";
}

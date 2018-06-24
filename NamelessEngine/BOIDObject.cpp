#include "BOIDObject.h"

BOIDObject::BOIDObject()
{
	this->addComponent(T);
	this->addComponent(Mr);
}


BOIDObject::~BOIDObject()
{
	//delete T;
	//delete Mr;
}

void BOIDObject::UpdateMeshRenderer()
{
	Mr->Shader = Shader;
	Mr->Mesh = Mesh;
	Mr->Material = Material;
}

void BOIDObject::UpdateTransform()
{
	T = new _NL::Component::Transform();
}


char * BOIDObject::getTypeName() 
{
	return this->name;
}

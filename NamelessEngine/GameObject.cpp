#include "GameObject.h"


_NL::Object::GameObject::GameObject()
{
}

_NL::Object::GameObject::GameObject(std::string name)
{
	Parent = 0;
	this->name = name;
	std::cout << this->getTypeName() << " " << this->name.c_str() << " was created sucessfully!" << std::endl;
}

_NL::Object::GameObject::GameObject(const _NL::Core::Object &CO)
{
	for (_NL::Core::Component* C : CO.Components)
	{
		if (C->getTypeName() == "_NL::Component::CppScript") {
			this->addComponent(static_cast<_NL::Component::CppScript<>*>(C)->clone());
		}
		else if (C->getTypeName() == "_NL::Component::Transform") {
			this->addComponent(new _NL::Component::Transform);
		}
		else if (C->getTypeName() == "_NL::Component::MeshRenderer") {
			this->addComponent(C);
		}
	}
}


char* _NL::Object::GameObject::getTypeName() const
{
	return "_NL::Object::GameObject";
}


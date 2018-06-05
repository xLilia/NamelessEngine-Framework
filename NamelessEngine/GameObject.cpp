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
	for each (_NL::Core::Component* C in CO.Components)
	{
		if (C->getTypeName() == "_NL::Component::CppScript") {
			this->addComponent(static_cast<_NL::Component::CppScript<>*>(C)->clone());
		}
		else if (C->getTypeName() == "_NL::Component::Transform") {
			this->addComponent(new _NL::Component::Transform);
		}
		else if (C->getTypeName() == "_NL::Component::MeshRenderer") {
			this->addComponent(C);
			//this->addComponent(new _NL::Component::MeshRenderer(*static_cast<_NL::Component::MeshRenderer*>(C)));
		}
	}
}

//void _NL::Object::GameObject::getInfo()
//{
//	std::cout << "|-START OBJECT LOG-|" << std::endl;
//	std::cout << "|--------------------------------------|" << std::endl;
//	std::cout << "|Object Name: " << this->name.c_str() << std::endl;
//	std::cout << "|Object Class: " << this->getTypeName() << std::endl;
//	std::cout << "|--------------------------------------|" << std::endl;
//	//std::cout << "|Is Active: " << bactive << std::endl;
//	//std::cout << "|Is Static: " << bstatic << std::endl;
//	//Parent
//	//Child tree
//	std::cout << "|--------------------------------------|" << std::endl;
//	std::cout << "|Atached Components: " << std::endl;
//	unsigned int i = 0;
//	for each (_NL::Core::Component* c in Components)
//	{
//		std::cout << "|" << i++ << " : "<< c->getTypeName() << std::endl;
//	}
//	std::cout << "|--------------------------------------|" << std::endl;
//	std::cout << "|-END-|" << std::endl;
//}

char* _NL::Object::GameObject::getTypeName() const
{
	return "_NL::Object::GameObject";
}


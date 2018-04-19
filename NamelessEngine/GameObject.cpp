#include "GameObject.h"

_NL::Object::GameObject::GameObject()
{
	this->name = "_";
}

_NL::Object::GameObject::GameObject(const GameObject &CO)
{
	for each (_NL::Core::Component* C in CO.Components)
	{
		if (C->ClassName() == "_NL::Component::Script") {
			//auto T = dynamic_cast<_NL::Component::Script<_NL::Core::CppScript>*>(C)->getScript();
			//
			//_NL::Component::Script<_NL::Core::CppScript>* N = new _NL::Component::Script<_NL::Core::CppScript>;
			//
			//N->CreateNewScript(T);
			
			//this->addComponent();
			//this->addComponent(new _NL::Component::Script<>);
		}
		if (C->ClassName() == "_NL::Component::Transform") {
			this->addComponent(new _NL::Component::Transform);
		}
		if (C->ClassName() == "_NL::Component::MeshRenderer") {
			this->addComponent(C);
		}
	}
}

_NL::Object::GameObject::GameObject(std::string name)
{
	Parent = 0;
	this->name = name;
	std::cout << this->ClassName().c_str()<< " " << this->name.c_str() << " was created sucessfully!" << std::endl;
}

void _NL::Object::GameObject::getInfo()
{
	std::cout << "|-START OBJECT LOG-|" << std::endl;
	std::cout << "|--------------------------------------|" << std::endl;
	std::cout << "|Object Name: " << this->name.c_str() << std::endl;
	std::cout << "|Object Class: " << this->ClassName().c_str() << std::endl;
	std::cout << "|--------------------------------------|" << std::endl;
	std::cout << "|Is Active: " << bactive << std::endl;
	std::cout << "|Is Static: " << bstatic << std::endl;
	//Parent
	//Child tree
	std::cout << "|--------------------------------------|" << std::endl;
	std::cout << "|Atached Components: " << std::endl;
	unsigned int i = 0;
	for each (_NL::Core::Component* c in Components)
	{
		std::cout << "|" << i++ << " : "<< c->ClassName().c_str() << std::endl;
	}
	std::cout << "|--------------------------------------|" << std::endl;
	std::cout << "|-END-|" << std::endl;
}

std::string _NL::Object::GameObject::ClassName() const
{
	return "_NL::Object::GameObject";
}

_NL::Object::GameObject::~GameObject()
{
}

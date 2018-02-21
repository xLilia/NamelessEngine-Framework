#include "GameObject.h"

_NL::Object::GameObject::GameObject()
{
	this->name = "_";
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

int _NL::Object::GameObject::addComponent(_NL::Core::Component *C)
{
	for each (_NL::Core::Component* c in Components)
	{
		if (c->ClassName() == C->ClassName()) {
			std::cout << "ERROR -1 :" << this->name.c_str() <<" Object Component List Already Has a " << C->ClassName().c_str() << " Component." << std::endl;
			return -1;
		}
	}
	Components.push_back(C);
	std::cout << C->ClassName().c_str() << " Component Added to " << this->name.c_str() << std::endl;
	
	return 0;
}

void _NL::Object::GameObject::StartScriptComponents()
{

	for each (_NL::Component::Script<_NL::Core::CppScript>* s in Components)
	{
		if (s->ClassName() == "_NL::Core::Component") {
			s->getScript()->Start();
		}

	}
}

void _NL::Object::GameObject::UpdateScriptComponents()
{
	for each (_NL::Component::Script<_NL::Core::CppScript>* s in Components)
	{
		//std::cout << s->ClassName().c_str();
		if (s->ClassName() == "_NL::Core::Component") { //OVERRIDE CLASSNAME BUG 
			s->getScript()->Update();
		}

	}
}

std::vector<_NL::Core::Component*> _NL::Object::GameObject::getComponents()
{
	return Components;
}

_NL::Object::GameObject::~GameObject()
{
}

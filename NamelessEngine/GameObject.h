#pragma once
#include <vector>
#include "NamelessCore.h"

class GameObject 
{
public:
	GameObject();
	void addComponent(_NLess::Component C);
	/*void setStatic();
	void setDynamic();*/
	std::vector<_NLess::Component> Components;
	GameObject *Parent;

	~GameObject();
};


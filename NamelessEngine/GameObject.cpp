#include "GameObject.h"


GameObject::GameObject()
{
}


void GameObject::addComponent(_NLess::Component C)
{
	Components.push_back(C);
}

GameObject::~GameObject()
{
}

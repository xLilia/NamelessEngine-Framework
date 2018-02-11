#include "WorldSpace.h"

_NL::Object::WorldSpace::WorldSpace()
{
	
}

void _NL::Object::WorldSpace::addObjectToWorld(_NL::Core::Object *G)
{
	ObjectList.push_back(G);
}

void _NL::Object::WorldSpace::showObjectList()
{
	std::cout << "|-START WORLD OBJList LOG-|" << std::endl;
	std::cout << "|--------------------------------------|" << std::endl;
	int i = 0;
	for each (_NL::Core::Object* o in ObjectList)
	{
		std::cout << "|" << i++ << " : " << o->name.c_str() << std::endl;
	}
	std::cout << "|--------------------------------------|" << std::endl;
}

std::vector<_NL::Core::Object*> _NL::Object::WorldSpace::GetObjectList()
{
	return this->ObjectList;
}


_NL::Object::WorldSpace::~WorldSpace()
{
}

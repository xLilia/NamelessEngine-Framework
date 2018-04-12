#include "WorldSpace.h"

_NL::Engine::WorldSpace::WorldSpace()
{

}

void _NL::Engine::WorldSpace::Instantiate(_NL::Object::GameObject * original, glm::vec3 position, glm::vec3 direction)
{
	int oid = 0;
	for each (std::vector<_NL::Core::Object*> obj in ObjectList)
	{
		if (original == obj[0]) {
			_NL::Object::GameObject* I = new _NL::Object::GameObject(*original);
			
			I->name += " clone(" + std::to_string(obj.size()) + ")";
			I->getComponent<_NL::Component::Transform>()->transform.position = position;
			I->getComponent<_NL::Component::Transform>()->transform.EulerRotation = direction;
			
			ObjectList[oid].push_back(I);
			//I->getComponent<_NL::Component::Script<_NL::Core::CppScript>>()->getScript()->Start();
		}
		oid++;
	}
}

void _NL::Engine::WorldSpace::addObjectToWorld(_NL::Core::Object *G)
{
	std::vector<_NL::Core::Object*> NewObj;
	NewObj.push_back(G);
	ObjectList.push_back(NewObj);
}

void _NL::Engine::WorldSpace::showObjectList()
{
	std::cout << "|-START WORLD OBJList LOG-|" << std::endl;
	std::cout << "|--------------------------------------|" << std::endl;
	int i = 0;
	for each (std::vector<_NL::Core::Object*> obj in ObjectList)
	{
		for each (_NL::Core::Object* o in obj)
		{
			std::cout << "|" << i++ << " : " << o->name.c_str() << std::endl;
		}
	}
	std::cout << "|--------------------------------------|" << std::endl;
}


_NL::Engine::WorldSpace::~WorldSpace()
{
}

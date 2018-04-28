#include "WorldSpace.h"

_NL::Engine::WorldSpace::WorldSpace()
{

}

_NL::Core::Object* _NL::Engine::WorldSpace::Instantiate(_NL::Core::Object* original, glm::vec3 position, glm::quat quaternionRotation, glm::vec3 scale)
{
	GLint objID = 0;
	_NL::Component::MeshRenderer* ObjMesh = original->getComponent<_NL::Component::MeshRenderer>();
	if (ObjMesh != nullptr)
	for each (std::vector<_NL::Core::Object*> obj in ObjectList)
	{
		GLuint last = obj.size();
		_NL::Component::MeshRenderer* checkMesh = obj[0]->getComponent<_NL::Component::MeshRenderer>();
		if (checkMesh != nullptr)
		if (ObjMesh == checkMesh) {
			ObjectList[objID].push_back(new _NL::Object::GameObject(*original));
			
			ObjectList[objID][last]->name += " clone(" + std::to_string(obj.size()) + ")";
			ObjectList[objID][last]->getComponent<_NL::Component::Transform>()->transform.position = position;
			ObjectList[objID][last]->getComponent<_NL::Component::Transform>()->transform.QuaternionRotation = quaternionRotation;
			ObjectList[objID][last]->getComponent<_NL::Component::Transform>()->transform.scale = scale;
			for each (_NL::Component::CppScript<_NL::Core::Script>* s in ObjectList[objID][last]->Components)
			{
				if (s->ClassName() == "_NL::Component::CppScript") {
					s->getScript()->awake = true;
					s->getScript()->_this = ObjectList[objID][last];
					s->getScript()->Start();
				}
			}
			//ObjectList[objID].push_back(I);
			return ObjectList[objID][last];
		}
		objID++;
	}
	this->addObjectToWorld(original);
	return original;
}

void _NL::Engine::WorldSpace::KillObjectInstance(_NL::Core::Object * object)
{
	for (int o = 0; o < ObjectList.size(); o++) {
		for (int i = 0; i < ObjectList[o].size(); i++) {
			if (ObjectList[o][i] == object) {
				ObjectList[o].erase(std::remove(ObjectList[o].begin(), ObjectList[o].end(), ObjectList[o][i]), ObjectList[o].end());
			}
		}
	}
}

void _NL::Engine::WorldSpace::addObjectToWorld(_NL::Core::Object *G)
{
	_NL::Component::MeshRenderer* ObjMesh = G->getComponent<_NL::Component::MeshRenderer>();
	GLint n = 0;
	if(ObjMesh != nullptr)
	for each (std::vector<_NL::Core::Object*> obj in ObjectList)
	{
		_NL::Component::MeshRenderer* checkMesh = obj[0]->getComponent<_NL::Component::MeshRenderer>();
		if (checkMesh != nullptr)
		if (ObjMesh->Mesh == checkMesh->Mesh) {
			ObjectList[n].push_back(G);
			return;
		}
		n++;
	}

	std::vector<_NL::Core::Object*> NewObj;
	NewObj.push_back(G);
	ObjectList.push_back(NewObj);
	return;
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

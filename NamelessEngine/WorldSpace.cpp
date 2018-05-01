#include "WorldSpace.h"

_NL::Engine::WorldSpace::WorldSpace()
{

}

_NL::Core::Object* _NL::Engine::WorldSpace::Instantiate(_NL::Core::Object* original, glm::vec3 position, glm::quat quaternionRotation, glm::vec3 scale)
{
	_NL::Component::MeshRenderer* ObjMesh = original->getComponent<_NL::Component::MeshRenderer>();
	if (ObjMesh != nullptr)
	for(int obj = 0; obj < ObjectList.size(); obj++)
	//for each (std::vector<_NL::Core::Object*> obj in ObjectList)
	{
		GLuint last = ObjectList[obj].size();
		_NL::Component::MeshRenderer* checkMesh = ObjectList[obj][0]->getComponent<_NL::Component::MeshRenderer>();
		if (checkMesh != nullptr)
		if (ObjMesh == checkMesh) {
			ObjectList[obj].push_back(new _NL::Object::GameObject(*original));
			
			 ObjectList[obj][last]->name += " clone(" + std::to_string(ObjectList[obj].size()) + ")";
			 ObjectList[obj][last]->getComponent<_NL::Component::Transform>()->transform.position = position;
			 ObjectList[obj][last]->getComponent<_NL::Component::Transform>()->transform.QuaternionRotation = quaternionRotation;
			 ObjectList[obj][last]->getComponent<_NL::Component::Transform>()->transform.scale = scale;
			for(int script = 0; script < ObjectList[obj][last]->Components.size(); script++)
			//for each (_NL::Component::CppScript<_NL::Core::Script>* s in ObjectList[objID][last]->Components)
			{
				_NL::Component::CppScript<_NL::Core::Script>& s = *static_cast<_NL::Component::CppScript<_NL::Core::Script>*>(ObjectList[obj][last]->Components[script]);
				if (s.ClassName() == "_NL::Component::CppScript") {
					s.getScript()->awake = true;
					s.getScript()->_this = ObjectList[obj][last];
					s.getScript()->Start();
				}
			}
			return  ObjectList[obj][last];
		}
	}

	this->addObjectToWorld(original);
	return original;
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

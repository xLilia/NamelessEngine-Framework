#include "WorldSpace.h"

_NL::Engine::WorldSpace::WorldSpace()
{
	
}

//_NL::Core::Object* _NL::Engine::WorldSpace::Instantiate(_NL::Core::Object* original, glm::vec3 position, glm::quat quaternionRotation, glm::vec3 scale)
//{
//	//_NL::Component::MeshRenderer* ObjMesh = original->getComponent<_NL::Component::MeshRenderer>();
//	//if (ObjMesh != nullptr)
//	//for(int obj = 0; obj < ObjectList.size(); obj++)
//	////for each (std::vector<_NL::Core::Object*> obj in ObjectList)
//	//{
//	//	GLuint last = ObjectList[obj].size();
//	//	_NL::Component::MeshRenderer* checkMesh = ObjectList[obj][0]->getComponent<_NL::Component::MeshRenderer>();
//	//	if (checkMesh != nullptr)
//	//	if (ObjMesh == checkMesh) {
//	//		ObjectList[obj].push_back(new _NL::Object::GameObject(*original));
//	//		
//	//		 ObjectList[obj][last]->name += " clone(" + std::to_string(ObjectList[obj].size()) + ")";
//	//		 ObjectList[obj][last]->getComponent<_NL::Component::Transform>()->transform.position = position;
//	//		 ObjectList[obj][last]->getComponent<_NL::Component::Transform>()->transform.QuaternionRotation = quaternionRotation;
//	//		 ObjectList[obj][last]->getComponent<_NL::Component::Transform>()->transform.scale = scale;
//	//		for(int script = 0; script < ObjectList[obj][last]->Components.size(); script++)
//	//		//for each (_NL::Component::CppScript<_NL::Core::Script>* s in ObjectList[objID][last]->Components)
//	//		{
//	//			_NL::Component::CppScript<_NL::Core::Script>& s = *static_cast<_NL::Component::CppScript<_NL::Core::Script>*>(ObjectList[obj][last]->Components[script]);
//	//			if (s.getTypeName() == "_NL::Component::CppScript") {
//	//				s.getScript()->awake = true;
//	//				s.getScript()->_this = ObjectList[obj][last];
//	//				s.getScript()->Start();
//	//			}
//	//		}
//	//		return  ObjectList[obj][last];
//	//	}
//	//}
//
//	original
//
//	_NL::Core::ObjInstanceList* OIL = getObjInstanceListLocation();
//
//	return OIL;
//}

_NL::Core::ObjTypeList* _NL::Engine::WorldSpace::addObjTypeListLocation(_NL::Core::Object *G) 
{
	std::string ObjType = G->getTypeName();
	//If Type not found, Expand types;
	ObjTypes.push_back(ObjType);
	ObjectLists.push_back(_NL::Core::ObjTypeList());
	return &ObjectLists.at(ObjectLists.size() - 1);
}

_NL::Core::ObjTypeList * _NL::Engine::WorldSpace::addObjTypeListLocation(std::string ObjType)
{
	//If Type not found, Expand types;
	ObjTypes.push_back(ObjType);
	ObjectLists.push_back(_NL::Core::ObjTypeList());
	return &ObjectLists.at(ObjectLists.size() - 1);
}

_NL::Core::ObjTypeList* _NL::Engine::WorldSpace::getObjTypeListLocation(_NL::Core::Object *G) 
{
	std::string ObjType = G->getTypeName();
	for (GLuint t = 0; t < ObjTypes.size(); t++) {
		if (ObjTypes.at(t) == ObjType) {
			return &ObjectLists[t];
		}
	}
	return nullptr;
}

_NL::Core::ObjTypeList* _NL::Engine::WorldSpace::getObjTypeListLocation(std::string ObjType)
{
	for (GLuint t = 0; t < ObjTypes.size(); t++) {
		if (ObjTypes.at(t) == ObjType) {
			return &ObjectLists[t];
		}
	}
	return nullptr;
}

_NL::Core::ObjInstanceList * _NL::Engine::WorldSpace::addObjInstanceListLocation(_NL::Core::Object * G, _NL::Core::ObjTypeList* T)
{
	if (T == nullptr) T = getObjTypeListLocation(G);

	//Add New InstanceList;
	_NL::Core::ObjInstanceList newObjID;
	//Add New Instance To List;
	newObjID.push_back(G);
	//Add InstanceList to ObjTypeList;
	T->push_back(newObjID);
	return &T->at(T->size() - 1);
}

_NL::Core::ObjInstanceList * _NL::Engine::WorldSpace::getObjInstanceListLocation(_NL::Core::Object * G, _NL::Core::ObjTypeList* T)
{
	if(T == nullptr) T = getObjTypeListLocation(G);

	_NL::Component::MeshRenderer* ObjMesh = G->getComponent<_NL::Component::MeshRenderer>();

	if (ObjMesh != nullptr) { //CONDITIONS TO BE INSTANTIATED;
		if (!ObjMesh->bIsUnpacked) {
			std::cout << "initializing MeshRenderer of Obj: " << G->name << std::endl;
			ObjMesh->initGLObj();
		}
		for (_NL::Core::ObjInstanceList& OIL : *T) {
			_NL::Component::MeshRenderer* CompareMesh = OIL[0]->getComponent<_NL::Component::MeshRenderer>();
			if (CompareMesh == ObjMesh) {
				OIL.push_back(G);
				return &OIL;//Instance Found
			}
		}	
	}
	return nullptr;
}

_NL::Core::ObjInstanceList* _NL::Engine::WorldSpace::addObjectToWorld(_NL::Core::Object *G)
{

	//Add Type if new;
	_NL::Core::ObjTypeList* T = getObjTypeListLocation(G);
	if (T == nullptr) T = addObjTypeListLocation(G);

	//Add Obj if new;
	_NL::Core::ObjInstanceList* OIL = getObjInstanceListLocation(G,T);
	if (OIL == nullptr) OIL = addObjInstanceListLocation(G, T);
	return OIL;
}

_NL::Engine::WorldSpace::~WorldSpace()
{
}

#include "NL.hpp"

int main(){

	_NL::Engine::WindowManager winMan("w1", 200, 200, false);

	_NL::Object::WorldSpace scene1;

	_NL::Object::GameObject* Obj1 = new _NL::Object::GameObject("nameless");
	
	_NL::Component::Transform* T = new _NL::Component::Transform;
	Obj1->addComponent(T);

	_NL::Component::Transform* getT = Obj1->getComponent(_NL::Component::Transform());
	_NL::Component::MeshRenderer* getM = Obj1->getComponent(_NL::Component::MeshRenderer());

	_NL::Component::MeshRenderer* M = new _NL::Component::MeshRenderer;
	Obj1->addComponent(M);
	getM = Obj1->getComponent(_NL::Component::MeshRenderer());

	Obj1->getInfo();

	scene1.addObjectToWorld(Obj1);
	scene1.showObjectList();

	//_NL::Component::MeshRenderer M;
	//Obj1.addComponent(M);
	//scene1.addGameObjectToWorld(Obj1);
	system("pause");
	return 0;
}
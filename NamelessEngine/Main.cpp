#include "NL.hpp"

int main(){

	_NL::Engine::WindowManager winMan("w1", 200, 200, false);

	_NL::Object::WorldSpace scene1;

	_NL::Object::GameObject* Obj1 = new _NL::Object::GameObject("nameless");
	
	Obj1->addComponent(new _NL::Component::Transform);
	Obj1->addComponent(new _NL::Component::MeshRenderer);
	
	Obj1->getComponent(_NL::Component::Transform())->transform.position += glm::vec3(1,2,3);

	Obj1->getComponent(_NL::Component::MeshRenderer())->LoadOBJFile("tri.obj");

	Obj1->getInfo();

	scene1.addObjectToWorld(Obj1);
	scene1.showObjectList();


	//_NL::Component::MeshRenderer M;
	//Obj1.addComponent(M);
	//scene1.addGameObjectToWorld(Obj1);
	system("pause");
	return 0;
}
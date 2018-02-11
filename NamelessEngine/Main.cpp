#include "NL.hpp"

int main(){

	_NL::Engine::WindowManager winMan("w1", 640, 480, true);
	_NL::Object::WorldSpace* scene1 = new _NL::Object::WorldSpace;
	_NL::Object::CameraObj* MyCam = new _NL::Object::CameraObj("MyCam", winMan.window->getSize().x/2, winMan.window->getSize().y,0,0,90,0.009,500);
	_NL::Object::CameraObj* MyCam2 = new _NL::Object::CameraObj("MyCam", winMan.window->getSize().x / 2, winMan.window->getSize().y, winMan.window->getSize().x / 2, 0, 90, 0.009, 500);


	_NL::Object::GameObject* Obj1 = new _NL::Object::GameObject("nameless");
	Obj1->addComponent(new _NL::Component::Transform);
	Obj1->addComponent(new _NL::Component::MeshRenderer);
	_NL::Element::MeshObj* Mesh1 = new _NL::Element::MeshObj("m1");
	_NL::Element::ShaderObj* Shade1 = new _NL::Element::ShaderObj("s1");
	Obj1->getComponent(_NL::Component::MeshRenderer())->Mesh = Mesh1;
	Obj1->getComponent(_NL::Component::MeshRenderer())->Shader = Shade1;
	Mesh1->LoadOBJFile("cube.obj");
	Shade1->InstallShader("defaultvertexshader.glsl", "defaultfragmentshader.glsl");
	
	_NL::Object::GameObject* Obj2 = new _NL::Object::GameObject("Tri");
	Obj2->addComponent(new _NL::Component::Transform());
	Obj2->addComponent(new _NL::Component::MeshRenderer());
	Obj2->getComponent(_NL::Component::MeshRenderer())->Mesh = new _NL::Element::MeshObj("trimesh", "tri.obj");
	Obj2->getComponent(_NL::Component::MeshRenderer())->Shader = new _NL::Element::ShaderObj("trishade", "defaultvertexshader.glsl", "blueFrag.glsl");
	
	_NL::Object::GameObject* Obj3 = new _NL::Object::GameObject("Quad");
	Obj3->addComponent(new _NL::Component::Transform());
	Obj3->addComponent(new _NL::Component::MeshRenderer());
	Obj3->getComponent(_NL::Component::MeshRenderer())->Mesh = new _NL::Element::MeshObj("quadmesh", "quad.obj");
	Obj3->getComponent(_NL::Component::MeshRenderer())->Shader = new _NL::Element::ShaderObj("quadshade", "defaultvertexshader.glsl", "grayFrag.glsl");

	//SCRIPT

	_NL::Component::Transform* T = Obj2->getComponent(_NL::Component::Transform());
	T->transform.scale *= .5;

	Obj2->getComponent(_NL::Component::Transform())->transform.position.x += -2;
	Obj2->getComponent(_NL::Component::Transform())->transform.scale *= 2;

	Obj3->getComponent(_NL::Component::Transform())->transform.position.y -= 1;
	Obj3->getComponent(_NL::Component::Transform())->transform.scale *= 5;

	MyCam->Transform.position.z += -1;
	MyCam->Transform.position.y += 2;
	MyCam->Transform.LookAtCenter = MyCam->Transform.LookAtCenter - -glm::vec3(0,-2,0);
	
	MyCam2->Transform.position.z += -2;

	//SETUP SCENE
	
	scene1->addObjectToWorld(Obj3);
	scene1->addObjectToWorld(Obj1);
	scene1->addObjectToWorld(Obj2);
	scene1->addObjectToWorld(MyCam);
	scene1->addObjectToWorld(MyCam2);
	
	scene1->showObjectList();
	//Obj1->getInfo();

	winMan.CurrentScene = scene1;
	winMan.RunGameLoop();
	return 0;
}
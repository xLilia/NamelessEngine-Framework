#include "NL.hpp"

int main(){

	_NL::Engine::WindowManager winMan("w1", 640, 480, true);
	_NL::Object::WorldSpace* scene1 = new _NL::Object::WorldSpace;
	_NL::Object::CameraObj* MyCam = new _NL::Object::CameraObj("MyCam", winMan.window->getSize().x, winMan.window->getSize().y);
	
	_NL::Object::GameObject* Obj1 = new _NL::Object::GameObject("nameless");
	Obj1->addComponent(new _NL::Component::Transform);
	Obj1->addComponent(new _NL::Component::MeshRenderer);
	_NL::Element::MeshObj* Mesh1 = new _NL::Element::MeshObj("m1");
	_NL::Element::ShaderObj* Shade1 = new _NL::Element::ShaderObj("s1");
	Obj1->getComponent(_NL::Component::MeshRenderer())->Mesh = Mesh1;
	Obj1->getComponent(_NL::Component::MeshRenderer())->Shader = Shade1;
	Mesh1->LoadOBJFile("cubeT.obj");
	Shade1->InstallShader("defaultvertexshader.glsl", "defaultfragmentshader.glsl");
	
	_NL::Object::GameObject* Obj2 = new _NL::Object::GameObject("Tri");
	Obj2->addComponent(new _NL::Component::Transform());
	Obj2->addComponent(new _NL::Component::MeshRenderer());
	Obj2->getComponent(_NL::Component::MeshRenderer())->Mesh = new _NL::Element::MeshObj("trimesh", "tri.obj");
	Obj2->getComponent(_NL::Component::MeshRenderer())->Shader = new _NL::Element::ShaderObj("trishade", "defaultvertexshader.glsl", "blueFrag.glsl");
	
	

	scene1->addObjectToWorld(Obj1);
	scene1->addObjectToWorld(Obj2);

	scene1->addObjectToWorld(MyCam);
	
	scene1->showObjectList();
	Obj1->getInfo();

	winMan.CurrentScene = scene1;
	winMan.CurrentScene->MainCamera = MyCam;
	winMan.RunGameLoop();
	return 0;
}
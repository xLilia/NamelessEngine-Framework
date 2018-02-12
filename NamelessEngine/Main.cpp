#include "NL.hpp"

int main(){

	_NL::Engine::WindowManager winMan("w1", 640, 480, false);
	_NL::Object::WorldSpace* scene1 = new _NL::Object::WorldSpace;
	_NL::Object::CameraObj* MyCam = new _NL::Object::CameraObj("MyCam", winMan.window->getSize().x, winMan.window->getSize().y,0,0,90,0.009,500);
	_NL::Object::CameraObj* MyCam2 = new _NL::Object::CameraObj("MyCam2", winMan.window->getSize().x / 2, winMan.window->getSize().y, winMan.window->getSize().x / 2, 0, 90, 1.0f, 500.0f);


	_NL::Object::GameObject* Cube = new _NL::Object::GameObject("nameless");
	Cube->addComponent(new _NL::Component::Transform);
	Cube->addComponent(new _NL::Component::MeshRenderer);
	_NL::Element::MeshObj* Mesh1 = new _NL::Element::MeshObj("m1");
	_NL::Element::ShaderObj* Shade1 = new _NL::Element::ShaderObj("s1");
	Cube->getComponent(_NL::Component::MeshRenderer())->Mesh = Mesh1;
	Cube->getComponent(_NL::Component::MeshRenderer())->Shader = Shade1;
	Mesh1->LoadOBJFile("cube.obj");
	Shade1->InstallShader("defaultvertexshader.glsl", "defaultfragmentshader.glsl");
	
	_NL::Object::GameObject* Tri = new _NL::Object::GameObject("Tri");
	Tri->addComponent(new _NL::Component::Transform());
	Tri->addComponent(new _NL::Component::MeshRenderer());
	Tri->getComponent(_NL::Component::MeshRenderer())->Mesh = new _NL::Element::MeshObj("trimesh", "tri.obj");
	Tri->getComponent(_NL::Component::MeshRenderer())->Shader = new _NL::Element::ShaderObj("trishade", "defaultvertexshader.glsl", "blueFrag.glsl");
	
	_NL::Object::GameObject* Quad = new _NL::Object::GameObject("Quad");
	Quad->addComponent(new _NL::Component::Transform());
	Quad->addComponent(new _NL::Component::MeshRenderer());
	Quad->getComponent(_NL::Component::MeshRenderer())->Mesh = new _NL::Element::MeshObj("quadmesh", "quad.obj");
	Quad->getComponent(_NL::Component::MeshRenderer())->Shader = new _NL::Element::ShaderObj("quadshade", "defaultvertexshader.glsl", "grayFrag.glsl");

	//SCRIPT

	_NL::Component::Transform* T = Tri->getComponent(_NL::Component::Transform());
	T->transform.scale *= .5;

	//Tri->getComponent(_NL::Component::Transform())->transform.position.z += 1;
	Tri->getComponent(_NL::Component::Transform())->transform.scale *= 5;

	Quad->getComponent(_NL::Component::Transform())->transform.position.y -= 1;
	Quad->getComponent(_NL::Component::Transform())->transform.scale *= 5;

	MyCam->Transform.position.z += -1;
	MyCam->Transform.position.y += 2;
	MyCam->Transform.LookAtCenter = MyCam->Transform.LookAtCenter +glm::vec3(0,-1.5,0);
	
	MyCam2->Transform.position.z += -2;

	//SETUP SCENE
	
	scene1->addObjectToWorld(Quad);
	scene1->addObjectToWorld(Cube);
	scene1->addObjectToWorld(Tri);
	scene1->addObjectToWorld(MyCam);
	//scene1->addObjectToWorld(MyCam2);
	
	scene1->showObjectList();
	//Obj1->getInfo();

	winMan.CurrentScene = scene1;
	winMan.RunGameLoop();
	return 0;
}
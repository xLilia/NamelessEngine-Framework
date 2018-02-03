#include "NL.hpp"

int main(){

	_NL::Engine::WindowManager winMan("w1", 640, 480, false);

	_NL::Object::WorldSpace scene1;
	_NL::Object::CameraObj MyCam(;

	_NL::Object::GameObject* Obj1 = new _NL::Object::GameObject("nameless");
	

	Obj1->addComponent(new _NL::Component::Transform);
	Obj1->addComponent(new _NL::Component::MeshRenderer);

	_NL::Element::MeshObj* Mesh1 = new _NL::Element::MeshObj("m1");
	_NL::Element::ShaderObj* Shade1 = new _NL::Element::ShaderObj("s1");
	Obj1->getComponent(_NL::Component::MeshRenderer())->Mesh=Mesh1;
	Obj1->getComponent(_NL::Component::MeshRenderer())->Shader=Shade1;
	
	Mesh1->LoadOBJFile("CubeT.obj");
	Shade1->InstallShader("defaultvertexshader.glsl", "defaultfragmentshader.glsl");
	
	scene1.addObjectToWorld(Obj1);
	
	scene1.showObjectList();
	Obj1->getInfo();

	winMan.RunGameLoop();

	system("pause");
	return 0;
}
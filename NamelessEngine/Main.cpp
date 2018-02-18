#include "NL.hpp"

int main(){

	_NL::Engine::WindowManager winMan("w1", 640, 480,true,true,0,1);
	_NL::Object::WorldSpace* scene1 = new _NL::Object::WorldSpace;
	_NL::Object::CameraObj* MyCam = new _NL::Object::CameraObj("MyCam", winMan.window->getSize().x, winMan.window->getSize().y,0,0,90,0.1,500);
	//_NL::Object::CameraObj* MyCam2 = new _NL::Object::CameraObj("MyCam2", winMan.window->getSize().x / 2, winMan.window->getSize().y, winMan.window->getSize().x / 2, 0, 90, 1.0f, 500.0f);
	
	_NL::Element::MeshObj* cubemesh = new _NL::Element::MeshObj("m1", "cubeMT.obj");
	_NL::Element::ShaderObj* defaultshader = new _NL::Element::ShaderObj("s1", "defaultvertexshader.glsl", "defaultfragmentshader.glsl");
	_NL::Element::MaterialObj* material1 = new _NL::Element::MaterialObj("mtl1", "CubeMT.mtl");
	_NL::Element::MaterialObj* material2 = new _NL::Element::MaterialObj("mtl2","skybox.mtl");
	
	_NL::Object::GameObject* skybox = new _NL::Object::GameObject("Skybox");
	skybox->addComponent(new _NL::Component::Transform);
	skybox->addComponent(new _NL::Component::MeshRenderer);
	skybox->getComponent(_NL::Component::MeshRenderer())->Mesh = cubemesh;
	skybox->getComponent(_NL::Component::MeshRenderer())->Shader = defaultshader;
	skybox->getComponent(_NL::Component::MeshRenderer())->Material = material2;

	_NL::Object::GameObject* Cube = new _NL::Object::GameObject("nameless");
	Cube->addComponent(new _NL::Component::Transform);
	Cube->addComponent(new _NL::Component::MeshRenderer);
	Cube->getComponent(_NL::Component::MeshRenderer())->Mesh = cubemesh;
	Cube->getComponent(_NL::Component::MeshRenderer())->Shader = defaultshader;
	Cube->getComponent(_NL::Component::MeshRenderer())->Material = material1;
	
	_NL::Object::GameObject* Cube2 = new _NL::Object::GameObject("nameless1");
	Cube2->addComponent(new _NL::Component::Transform);
	Cube2->addComponent(new _NL::Component::MeshRenderer);
	Cube2->getComponent(_NL::Component::MeshRenderer())->Mesh = cubemesh;
	Cube2->getComponent(_NL::Component::MeshRenderer())->Shader = defaultshader;
	Cube2->getComponent(_NL::Component::MeshRenderer())->Material = material1;

	_NL::Object::GameObject* Cube3 = new _NL::Object::GameObject("nameless1");
	Cube3->addComponent(new _NL::Component::Transform);
	Cube3->addComponent(new _NL::Component::MeshRenderer);
	Cube3->getComponent(_NL::Component::MeshRenderer())->Mesh = cubemesh;
	Cube3->getComponent(_NL::Component::MeshRenderer())->Shader = defaultshader;
	Cube3->getComponent(_NL::Component::MeshRenderer())->Material = material1;

	_NL::Object::GameObject* Tri = new _NL::Object::GameObject("Tri");
	Tri->addComponent(new _NL::Component::Transform());
	Tri->addComponent(new _NL::Component::MeshRenderer());
	Tri->getComponent(_NL::Component::MeshRenderer())->Mesh = new _NL::Element::MeshObj("trimesh", "tri.obj");
	_NL::Element::ShaderObj* trishade = new _NL::Element::ShaderObj("trishade", "defaultvertexshader.glsl", "blueFrag.glsl");
	Tri->getComponent(_NL::Component::MeshRenderer())->Shader = trishade;
	Tri->getComponent(_NL::Component::MeshRenderer())->Material = material1;

	_NL::Object::GameObject* Quad = new _NL::Object::GameObject("Quad");
	Quad->addComponent(new _NL::Component::Transform());
	Quad->addComponent(new _NL::Component::MeshRenderer());
	Quad->getComponent(_NL::Component::MeshRenderer())->Mesh = new _NL::Element::MeshObj("quadmesh", "quad.obj");
	Quad->getComponent(_NL::Component::MeshRenderer())->Shader = defaultshader;//new _NL::Element::ShaderObj("quadshade", "defaultvertexshader.glsl", "grayFrag.glsl");
	Quad->getComponent(_NL::Component::MeshRenderer())->Material = material1;
	
	_NL::Object::LightObject* Light = new _NL::Object::LightObject("Light");
	//Light->addComponent(new _NL::Component::MeshRenderer());
	//Light->getComponent(_NL::Component::MeshRenderer())->Mesh = cubemesh;
	//Light->getComponent(_NL::Component::MeshRenderer())->Shader = trishade;
	
	//SCRIPT

	Light->LightProperties.lightPosition.y = 5;
	//Light->LightProperties.position.z = 0;
	Light->LightProperties.lightColor = glm::vec4(1,1,1,1);

	skybox->getComponent(_NL::Component::Transform())->transform.scale *= 100;

	Cube2->getComponent(_NL::Component::Transform())->transform.position.x += 2;
	Cube2->getComponent(_NL::Component::Transform())->transform.position.z += 2;
	Cube2->getComponent(_NL::Component::Transform())->transform.position.y += 2;
	Cube2->getComponent(_NL::Component::Transform())->transform.scale.y += 7;
	Cube2->Parent = Cube;

	Cube3->getComponent(_NL::Component::Transform())->transform.position.x -= 2;
	Cube3->getComponent(_NL::Component::Transform())->transform.position.z += 2;
	Cube3->getComponent(_NL::Component::Transform())->transform.position.y += 2;
	Cube3->getComponent(_NL::Component::Transform())->transform.scale.y += 7;
	//Cube3->Parent = Cube;

	_NL::Component::Transform* T = Tri->getComponent(_NL::Component::Transform());
	T->transform.scale *= .5;
	T->transform.position.y += 2;
	T->transform.position.z += 2;
	T->transform.scale *= 5;
	Tri->Parent = 0;

	Quad->getComponent(_NL::Component::Transform())->transform.position.y -= 1;
	Quad->getComponent(_NL::Component::Transform())->transform.scale *= 5;

	MyCam->Transform.position.z += -1;
	MyCam->Transform.position.y += 1;
	//MyCam->Transform.LookAtCenter = MyCam->Transform.LookAtCenter +glm::vec3(0,-1.5,0);
	//MyCam2->Transform.position.z += -2;

	//SETUP SCENE
	
	scene1->addObjectToWorld(Quad);
	scene1->addObjectToWorld(Cube);
	scene1->addObjectToWorld(Cube2);
	scene1->addObjectToWorld(Cube3);
	scene1->addObjectToWorld(Tri);
	scene1->addObjectToWorld(skybox);
	scene1->addObjectToWorld(MyCam);
	scene1->addObjectToWorld(Light);
	
	scene1->showObjectList();
	//Obj1->getInfo();

	winMan.CurrentScene = scene1;
	winMan.RunGameLoop();
	return 0;
}
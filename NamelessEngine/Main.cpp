#pragma once
#include "NL.hpp"
#include "TemplateScript.hpp"
#include "CamController.hpp"


int main(){
	_NL::Engine::WindowManager winMan("w1", 640, 480,true,true);
	_NL::Engine::WorldSpace* scene1 = new _NL::Engine::WorldSpace;
	_NL::Engine::WorldSpace* scene2 = new _NL::Engine::WorldSpace;
	_NL::Engine::AudioSource* Audio = new _NL::Engine::AudioSource;
	Audio->LoadAudioFile("deeplyMono.ogg");
	//Audio->Sound.play();

	_NL::Object::SkyboxObj* sky1 = new _NL::Object::SkyboxObj();
	_NL::Object::SkyboxObj* sky2= new _NL::Object::SkyboxObj();
	_NL::Element::ShaderObj* SkyShade = new _NL::Element::ShaderObj("SkyboxDefaultVertshader.glsl", "SkyboxDefaultFragshader.glsl");
	sky1->SkyboxShader = SkyShade;
	sky2->SkyboxShader = SkyShade;
	sky1->createCubeMap("sky2/ft.tga", "sky2/bk.tga", "sky2/up.tga", "sky2/dn.tga", "sky2/lf.tga", "sky2/rt.tga");
	sky2->createCubeMap("sky1/fadeaway_ft.tga", "sky1/fadeaway_bk.tga", "sky1/fadeaway_up.tga", "sky1/fadeaway_dn.tga", "sky1/fadeaway_lf.tga", "sky1/fadeaway_rt.tga");
	//sky2->createCubeMap("grass.png", "grass.png", "grass.png", "grass.png", "grass.png", "grass.png");
	scene1->Skybox = sky1;
	scene2->Skybox = sky2;
	_NL::Object::CameraObj* MyCam = new _NL::Object::CameraObj("MyCam", winMan.window->getSize().x, winMan.window->getSize().y,0,0,2,0.1,500,1);
	MyCam->addComponent(new _NL::Component::Script<CamController>);
	MyCam->getComponent<_NL::Component::Script<CamController>>()->CreateScript(new CamController());
	MyCam->getComponent<_NL::Component::Script<CamController>>()->getScript()->_this = MyCam;
	MyCam->getComponent<_NL::Component::Script<CamController>>()->getScript()->W = &winMan;
	
	_NL::Object::CameraObj* MyCam2 = new _NL::Object::CameraObj("MyCam2", winMan.window->getSize().x / 2, winMan.window->getSize().y, winMan.window->getSize().x / 2, 0, 90, 0.1, 500, .5);

	_NL::Element::MeshObj* cubemesh = new _NL::Element::MeshObj("cubeMT.obj");

	//_NL::Element::MaterialObj* material1 = new _NL::Element::MaterialObj("CubeMT.mtl");
	_NL::Element::MaterialObj* material1 = new _NL::Element::MaterialObj();
	material1->Add_NewMaterial();
	material1->Add_NewTexture("cyber-ground-emisiive.jpg");

	material1->MTLData[0].MTL_ID = 0;
	material1->MTLData[0].Ns = 100.0f; 
	material1->MTLData[0].Kd = glm::vec3(1.0f, 1.0f, 1.0f); 
	material1->MTLData[0].Ka = glm::vec3(0.5f); 
	material1->MTLData[0].Ke = glm::vec3(0.0f); 
	material1->MTLData[0].Ks = glm::vec3(0.5f); 

	_NL::Element::MeshObj* YazM = new _NL::Element::MeshObj("YazarusTaxon.obj");
	
	_NL::Element::MaterialObj* materialYT = new _NL::Element::MaterialObj();
	materialYT->Add_NewMaterial();
	materialYT->Add_NewTexture("noTex.png");

	materialYT->MTLData[0].MTL_ID = 0;
	materialYT->MTLData[0].Ns = 96.0f;
	materialYT->MTLData[0].Kd = glm::vec3(0.5f);
	materialYT->MTLData[0].Ka = glm::vec3(0.5f);
	materialYT->MTLData[0].Ke = glm::vec3(0.0f);
	materialYT->MTLData[0].Ks = glm::vec3(0.0f);

	materialYT->Add_NewMaterial();
	materialYT->Add_NewTexture("noTex.png");

	materialYT->MTLData[1].MTL_ID = 1;
	materialYT->MTLData[1].Ns = 96.0f;
	materialYT->MTLData[1].Kd = glm::vec3(1.0f);
	materialYT->MTLData[1].Ka = glm::vec3(1.0f);
	materialYT->MTLData[1].Ke = glm::vec3(0.0f);
	materialYT->MTLData[1].Ks = glm::vec3(0.0f);

	_NL::Element::ShaderObj* defaultshader = new _NL::Element::ShaderObj("defaultvertexshader.glsl", "defaultfragmentshader.glsl");
	_NL::Element::ShaderObj* trishade = new _NL::Element::ShaderObj("defaultvertexshader.glsl", "blueFrag.glsl");

	_NL::Object::GameObject* Yaz = new _NL::Object::GameObject("YazarusTaxon");
	Yaz->addComponent(new _NL::Component::Transform);
	Yaz->addComponent(new _NL::Component::MeshRenderer);
	Yaz->getComponent<_NL::Component::MeshRenderer>()->Mesh = YazM;
	Yaz->getComponent<_NL::Component::MeshRenderer>()->Shader = defaultshader;
	Yaz->getComponent<_NL::Component::MeshRenderer>()->Material = materialYT;

	_NL::Object::GameObject* Cube = new _NL::Object::GameObject("nameless");
	Cube->addComponent(new _NL::Component::Transform);
	Cube->addComponent(new _NL::Component::MeshRenderer);
	Cube->addComponent(new _NL::Component::Script<TemplateScript>);
	Cube->getComponent<_NL::Component::MeshRenderer>()->Mesh = cubemesh;
	Cube->getComponent<_NL::Component::MeshRenderer>()->Shader = defaultshader;
	Cube->getComponent<_NL::Component::MeshRenderer>()->Material = material1;
	Cube->getComponent<_NL::Component::Script<TemplateScript>>()->CreateScript(new TemplateScript());
	Cube->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->_this = Cube;
	Cube->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->W = &winMan;

	_NL::Object::GameObject* Cube2 = new _NL::Object::GameObject("nameless1");
	Cube2->addComponent(new _NL::Component::Transform);
	Cube2->addComponent(new _NL::Component::MeshRenderer);
	Cube2->getComponent<_NL::Component::MeshRenderer>()->Mesh = cubemesh;
	Cube2->getComponent<_NL::Component::MeshRenderer>()->Shader = defaultshader;
	Cube2->getComponent<_NL::Component::MeshRenderer>()->Material = material1;

	_NL::Object::GameObject* Cube3 = new _NL::Object::GameObject("nameless1");
	Cube3->addComponent(new _NL::Component::Transform);
	Cube3->addComponent(new _NL::Component::MeshRenderer);
	Cube3->getComponent<_NL::Component::MeshRenderer>()->Mesh = cubemesh;
	Cube3->getComponent<_NL::Component::MeshRenderer>()->Shader = defaultshader;
	Cube3->getComponent<_NL::Component::MeshRenderer>()->Material = material1;

	_NL::Object::GameObject* Tri = new _NL::Object::GameObject("Tri");
	Tri->addComponent(new _NL::Component::Transform());
	Tri->addComponent(new _NL::Component::MeshRenderer());
	Tri->addComponent(new _NL::Component::Script<TemplateScript>);
	Tri->getComponent<_NL::Component::MeshRenderer>()->Mesh = new _NL::Element::MeshObj("tri.obj");
	Tri->getComponent<_NL::Component::MeshRenderer>()->Shader = trishade;
	Tri->getComponent<_NL::Component::MeshRenderer>()->Material = material1;
	Tri->getComponent<_NL::Component::Script<TemplateScript>>()->CreateScript(new TemplateScript());
	Tri->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->_this = Tri;
	Tri->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->W = &winMan;

	_NL::Object::GameObject* Quad = new _NL::Object::GameObject("Quad");
	Quad->addComponent(new _NL::Component::Transform());
	Quad->addComponent(new _NL::Component::MeshRenderer());
	Quad->getComponent<_NL::Component::MeshRenderer>()->Mesh = new _NL::Element::MeshObj("quad.obj");
	Quad->getComponent<_NL::Component::MeshRenderer>()->Shader = defaultshader;//new _NL::Element::ShaderObj("quadshade", "defaultvertexshader.glsl", "grayFrag.glsl");
	Quad->getComponent<_NL::Component::MeshRenderer>()->Material = material1;
	
	_NL::Object::LightObject* Light = new _NL::Object::LightObject("Light");
	_NL::Object::LightObject* Light2 = new _NL::Object::LightObject("Light2");
	//Light->addComponent(new _NL::Component::MeshRenderer());
	//Light->getComponent(_NL::Component::MeshRenderer())->Mesh = cubemesh;
	//Light->getComponent(_NL::Component::MeshRenderer())->Shader = trishade;
	
	Yaz->getComponent<_NL::Component::Transform>()->transform.position.y += 5;

	Light->LightProperties.lightPosition.y = 0;
	Light->LightProperties.lightPosition.z = 2;
	Light->LightProperties.lightColor = glm::vec4(1, 0, 0, 1);
	Light->LightProperties.radiusOfInfluence = 4;

	Light2->LightProperties.lightPosition.y = 2;
	Light2->LightProperties.lightPosition.z = 2;
	Light2->LightProperties.lightColor = glm::vec4(1, 1, 1, 1);
	Light2->LightProperties.radiusOfInfluence = 1;

	Cube2->getComponent<_NL::Component::Transform>()->transform.position.x += 2;
	Cube2->getComponent<_NL::Component::Transform>()->transform.position.z += 2;
	Cube2->getComponent<_NL::Component::Transform>()->transform.position.y += 2;
	Cube2->getComponent<_NL::Component::Transform>()->transform.scale.y += 7;
	Cube2->Parent = Cube;

	Cube3->getComponent<_NL::Component::Transform>()->transform.position.x -= 2;
	Cube3->getComponent<_NL::Component::Transform>()->transform.position.z += 2;
	Cube3->getComponent<_NL::Component::Transform>()->transform.position.y += 2;
	Cube3->getComponent<_NL::Component::Transform>()->transform.scale.y += 7;
	Cube3->Parent = Cube;
	
	_NL::Component::Transform* T = Tri->getComponent<_NL::Component::Transform>();
	T->transform.scale *= .5;
	T->transform.position.y += 2;
	T->transform.position.z += 2;
	T->transform.scale *= 1;
	Tri->Parent = 0;

	Quad->getComponent<_NL::Component::Transform>()->transform.position.y -= 1;
	Quad->getComponent<_NL::Component::Transform>()->transform.scale *= 5;

	MyCam->Transform.position.z += -1;
	MyCam->Transform.position.y += 1;
	//MyCam->Transform.LookAtCenter = MyCam->Transform.LookAtCenter +glm::vec3(0,-1.5,0);
	MyCam2->Transform.position.z += -2;

	//SETUP SCENE
	
	scene1->addObjectToWorld(Yaz);
	scene1->addObjectToWorld(Quad);
	scene1->addObjectToWorld(Cube);
	scene1->addObjectToWorld(Cube2);
	scene1->addObjectToWorld(Cube3);
	scene1->addObjectToWorld(Tri);
	scene1->addObjectToWorld(MyCam);
	scene1->addObjectToWorld(Light);
	//scene1->addObjectToWorld(Light2);
	
	scene1->showObjectList();
	winMan.CurrentScene = scene1;
	winMan.RunCurrentScene();

	scene2->addObjectToWorld(Yaz);
	scene2->addObjectToWorld(Tri);
	scene2->addObjectToWorld(Quad);
	scene2->addObjectToWorld(MyCam);
	Quad->getComponent<_NL::Component::Transform>()->transform.scale *= 2;
	Yaz->getComponent<_NL::Component::Transform>()->transform.scale *= 2;
	Tri->getComponent<_NL::Component::Transform>()->transform.scale *= 100;
	Tri->getComponent<_NL::Component::Transform>()->transform.position.z = 10;
	Tri->getComponent<_NL::Component::Transform>()->transform.position.y = 50;
	winMan.ClearColor = glm::vec3(0.3, 0.4, 0.5);
	scene2->showObjectList();
	winMan.CurrentScene = scene2;
	winMan.RunCurrentScene();
	return 0;
}


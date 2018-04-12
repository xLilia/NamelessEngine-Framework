#pragma once
#include "NL.hpp"
#include "TemplateScript.hpp"
#include "CamController.hpp"


int main() {
	//check_gl_error_full();
	_NL::Engine::GameManager winMan("w1", 1024, 960, true, false);
	_NL::Engine::WorldSpace* scene1 = new _NL::Engine::WorldSpace;
	_NL::Engine::WorldSpace* scene2 = new _NL::Engine::WorldSpace;
	//_NL::Engine::AudioSource* Audio = new _NL::Engine::AudioSource;
	//Audio->LoadAudioFile("deeplyMono.ogg");
	//Audio->Sound.play();

	_NL::Object::SkyboxObj* sky1 = new _NL::Object::SkyboxObj();
	_NL::Element::ShaderInstance* SkyboxShader = new _NL::Element::ShaderInstance("SkyboxDefaultVertshader.glsl", "SkyboxDefaultFragshader.glsl");
	_NL::Element::ShaderInstance* HDRImageShader = new _NL::Element::ShaderInstance("HDRImageDefaultVertshader.glsl", "HDRImageDefaultFragshader.glsl");
	_NL::Element::ShaderInstance* EnvironmentShader = new _NL::Element::ShaderInstance("EnvironmentDefaultVertshader.glsl", "EnvironmentDefaultFragshader.glsl");
	sky1->SkyboxShader = SkyboxShader;
	sky1->HDRImageShader = HDRImageShader;
	sky1->EnvironmentShader = EnvironmentShader;
	
	//sky1->createSkybox(
	//	"sky2/ft.tga", 
	//	"sky2/bk.tga",
	//	"sky2/up.tga",
	//	"sky2/dn.tga", 
	//	"sky2/lf.tga", 
	//	"sky2/rt.tga"
	//);

	sky1->createEnvironment("hdr1/gunners_lounge.jpg", 1024*2);
	sky1->createSkybox("hdr1/gunners_lounge.jpg",1024*4);
	//sky1->createSkybox(
	//	"sky1/fadeaway_ft.tga", 
	//	"sky1/fadeaway_bk.tga", 
	//	"sky1/fadeaway_up.tga", 
	//	"sky1/fadeaway_dn.tga", 
	//	"sky1/fadeaway_lf.tga", 
	//	"sky1/fadeaway_rt.tga"
	//);
	
	scene1->Skybox = sky1;
	scene2->Skybox = sky1;

	_NL::Object::CameraObj* MyCam = new _NL::Object::CameraObj("MyCam", winMan.window->getSize().x, winMan.window->getSize().y, 0, 0, 90, 0.1, 500, 1);
	MyCam->addComponent(new _NL::Component::Script<CamController>);
	MyCam->getComponent<_NL::Component::Script<CamController>>()->CreateScript(new CamController());
	MyCam->getComponent<_NL::Component::Script<CamController>>()->getScript()->_this = MyCam;
	MyCam->getComponent<_NL::Component::Script<CamController>>()->getScript()->W = &winMan;

	_NL::Object::CameraObj* MyCam2 = new _NL::Object::CameraObj("MyCam2", winMan.window->getSize().x / 2, winMan.window->getSize().y, winMan.window->getSize().x / 2, 0, 90, 0.1, 500, .5);

	_NL::Element::MeshInstance* cubemesh = new _NL::Element::MeshInstance("cubeMT.obj");

	_NL::Tools::TextureLoader Textures;

	Textures.GenerateTexure("rustediron-streaks2-Unreal-Engine/rustediron-streaks_basecolor.png");
	Textures.GenerateTexure("rustediron-streaks2-Unreal-Engine/rustediron-streaks_Roughness.png");
	Textures.GenerateTexure("rustediron-streaks2-Unreal-Engine/rustediron-streaks_metallic.png");
	Textures.GenerateTexure("rustediron-streaks2-Unreal-Engine/rustediron-streaks_Normal.png");

	Textures.GenerateTexure("streakedmetal-ue/streakedmetal-albedo.png");
	Textures.GenerateTexure("streakedmetal-ue/streakedmetal-roughness.png");
	Textures.GenerateTexure("streakedmetal-ue/streakedmetal-metalness.png");
	Textures.GenerateTexure("defaultNormal.png");

	Textures.GenerateTexure("rock_vstreaks_ue/rock_vstreaks_Base_Color.png");
	Textures.GenerateTexure("rock_vstreaks_ue/rock_vstreaks_Roughness.png");
	Textures.GenerateTexure("rock_vstreaks_ue/rock_vstreaks_Metallic.png");
	Textures.GenerateTexure("rock_vstreaks_ue/rock_vstreaks_Normal-ue.png");
	Textures.GenerateTexure("rock_vstreaks_ue/rock_vstreaks_Ambient_Occlusion.png");
	
	_NL::Element::MaterialInstance* material1 = new _NL::Element::MaterialInstance();
	material1->AddNew_Material();
	material1->MaterialInstanceData[0].AlbedoTexId = Textures.GLTexIDs[0];
	material1->MaterialInstanceData[0].RoughnessTexId = Textures.GLTexIDs[1];
	material1->MaterialInstanceData[0].MetalnessTexId = Textures.GLTexIDs[2];
	material1->MaterialInstanceData[0].NormalTexId = Textures.GLTexIDs[3];
	material1->MaterialInstanceData[0].AmbientOculusionTexId = Textures.GLTexIDs[3];
	material1->MaterialInstanceData[0].MTL_ID = 0;

	_NL::Element::MeshInstance* YazM = new _NL::Element::MeshInstance("YazarusTaxon.obj");

	_NL::Element::MaterialInstance* materialSphere = new _NL::Element::MaterialInstance();
	materialSphere->AddNew_Material();
	materialSphere->MaterialInstanceData[0].AlbedoTexId = Textures.GLTexIDs[8];
	materialSphere->MaterialInstanceData[0].RoughnessTexId = Textures.GLTexIDs[9];
	materialSphere->MaterialInstanceData[0].MetalnessTexId = Textures.GLTexIDs[10];
	materialSphere->MaterialInstanceData[0].NormalTexId = Textures.GLTexIDs[11];
	materialSphere->MaterialInstanceData[0].AmbientOculusionTexId = Textures.GLTexIDs[12];
	materialSphere->MaterialInstanceData[0].MTL_ID = 0;

	_NL::Element::MeshInstance* Spheremesh = new _NL::Element::MeshInstance("Sphere.obj");
	

	_NL::Element::ShaderInstance* defaultshader = new _NL::Element::ShaderInstance("defaultvertexshader.glsl", "defaultfragmentshader.glsl");
	_NL::Element::ShaderInstance* trishade = new _NL::Element::ShaderInstance("defaultvertexshader.glsl", "blueFrag.glsl");

	_NL::Object::GameObject* Sphere = new _NL::Object::GameObject("Shpere");
	Sphere->addComponent(new _NL::Component::Transform);
	Sphere->addComponent(new _NL::Component::MeshRenderer);
	Sphere->addComponent(new _NL::Component::Script<TemplateScript>);
	Sphere->getComponent<_NL::Component::MeshRenderer>()->Mesh = Spheremesh;
	Sphere->getComponent<_NL::Component::MeshRenderer>()->Shader = defaultshader;
	Sphere->getComponent<_NL::Component::MeshRenderer>()->Material = materialSphere;
	Sphere->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->_this = Sphere;
	Sphere->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->W = &winMan;

	_NL::Object::GameObject* Yaz = new _NL::Object::GameObject("YazarusTaxon");
	Yaz->addComponent(new _NL::Component::Transform);
	Yaz->addComponent(new _NL::Component::MeshRenderer);
	Yaz->getComponent<_NL::Component::MeshRenderer>()->Mesh = YazM;
	Yaz->getComponent<_NL::Component::MeshRenderer>()->Shader = defaultshader;
	Yaz->getComponent<_NL::Component::MeshRenderer>()->Material = materialSphere;

	_NL::Object::GameObject* Cube = new _NL::Object::GameObject("cube");
	Cube->addComponent(new _NL::Component::Transform);
	Cube->addComponent(new _NL::Component::MeshRenderer);
	Cube->addComponent(new _NL::Component::Script<TemplateScript>);
	Cube->getComponent<_NL::Component::MeshRenderer>()->Mesh = cubemesh;
	Cube->getComponent<_NL::Component::MeshRenderer>()->Shader = defaultshader;
	Cube->getComponent<_NL::Component::MeshRenderer>()->Material = materialSphere;
	Cube->getComponent<_NL::Component::Script<TemplateScript>>()->CreateScript(new TemplateScript());
	Cube->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->_this = Cube;
	Cube->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->W = &winMan;

	_NL::Object::GameObject* Tri = new _NL::Object::GameObject("Tri");
	Tri->addComponent(new _NL::Component::Transform());
	Tri->addComponent(new _NL::Component::MeshRenderer());
	Tri->addComponent(new _NL::Component::Script<TemplateScript>);
	Tri->getComponent<_NL::Component::MeshRenderer>()->Mesh = new _NL::Element::MeshInstance("tri.obj");
	Tri->getComponent<_NL::Component::MeshRenderer>()->Shader = defaultshader;
	Tri->getComponent<_NL::Component::MeshRenderer>()->Material = material1;
	Tri->getComponent<_NL::Component::Script<TemplateScript>>()->CreateScript(new TemplateScript());
	Tri->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->_this = Tri;
	Tri->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->W = &winMan;

	_NL::Object::GameObject* Quad = new _NL::Object::GameObject("Quad");
	Quad->addComponent(new _NL::Component::Transform());
	Quad->addComponent(new _NL::Component::MeshRenderer());
	Quad->addComponent(new _NL::Component::Script<TemplateScript>);
	Quad->getComponent<_NL::Component::MeshRenderer>()->Mesh = new _NL::Element::MeshInstance("quad.obj");
	Quad->getComponent<_NL::Component::MeshRenderer>()->Shader = defaultshader;//new _NL::Element::ShaderInstance("quadshade", "defaultvertexshader.glsl", "grayFrag.glsl");
	Quad->getComponent<_NL::Component::MeshRenderer>()->Material = materialSphere;
	Quad->getComponent<_NL::Component::Script<TemplateScript>>()->CreateScript(new TemplateScript());
	Quad->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->_this = Quad;
	Quad->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->W = &winMan;
	
	_NL::Object::LightObject* Light = new _NL::Object::LightObject("Light");
	_NL::Object::LightObject* Light2 = new _NL::Object::LightObject("Light2");
	_NL::Object::LightObject* Light3 = new _NL::Object::LightObject("Light3");
	_NL::Object::LightObject* Light4 = new _NL::Object::LightObject("Light4");
	_NL::Object::LightObject* Light5 = new _NL::Object::LightObject("Light5");
	_NL::Object::LightObject* Light6 = new _NL::Object::LightObject("Light6");
	//Light->addComponent(new _NL::Component::MeshRenderer());
	//Light->getComponent(_NL::Component::MeshRenderer())->Mesh = cubemesh;
	//Light->getComponent(_NL::Component::MeshRenderer())->Shader = trishade;
	
	Yaz->getComponent<_NL::Component::Transform>()->transform.position.y += 5;

	Sphere->getComponent<_NL::Component::Transform>()->transform.position.y = 4;

	Light->LightProperties.lightPosition.y = 20;
	Light->LightProperties.lightPosition.x = 20;
	Light->LightProperties.lightPosition.z = 20;
	Light->LightProperties.lightColor = glm::vec3(1000, 1500, 1500);

	Light2->LightProperties.lightPosition.y = 8;
	Light2->LightProperties.lightPosition.x = 2;
	Light2->LightProperties.lightColor = glm::vec3(150, 150, 100);

	Light3->LightProperties.lightPosition.y = 2;
	Light3->LightProperties.lightPosition.x = 2;
	Light3->LightProperties.lightColor = glm::vec3(150, 100, 100);

	Light4->LightProperties.lightPosition.y = 2;
	Light4->LightProperties.lightPosition.x = -2;
	Light4->LightProperties.lightColor = glm::vec3(100, 100, 150);

	Light5->LightProperties.lightPosition.y = 4;
	Light5->LightProperties.lightPosition.z = -2;
	Light5->LightProperties.lightColor = glm::vec3(100, 150, 100);

	Light6->LightProperties.lightPosition.y = 4;
	Light6->LightProperties.lightPosition.z = 2;
	Light6->LightProperties.lightColor = glm::vec3(150, 100, 150);
	
	_NL::Component::Transform* T = Tri->getComponent<_NL::Component::Transform>();
	T->transform.scale *= .5;
	T->transform.position.y += 2;
	T->transform.position.z += 2;
	T->transform.scale *= 1;
	Tri->Parent = 0;

	T = Cube->getComponent<_NL::Component::Transform>();

	Quad->getComponent<_NL::Component::Transform>()->transform.position.y -= 1;
	Quad->getComponent<_NL::Component::Transform>()->transform.scale *= 200;

	MyCam->Transform.position.z += -1;
	MyCam->Transform.position.y += 1;
	//MyCam->Transform.LookAtCenter = MyCam->Transform.LookAtCenter +glm::vec3(0,-1.5,0);
	MyCam2->Transform.position.z += -2;

	//SETUP SCENE
	
	scene1->addObjectToWorld(Yaz);
	scene1->addObjectToWorld(Sphere);
	scene1->addObjectToWorld(Quad);
	//scene1->addObjectToWorld(Quad2);
	scene1->addObjectToWorld(Cube);
	
	
	for(int i = 0; i< 1000; i++){scene1->Instantiate(Yaz, T->transform.position + glm::vec3(rand() % 100, 2, rand() % 100), T->transform.EulerRotation);}


	//scene1->addObjectToWorld(Cube2);
	//scene1->addObjectToWorld(Cube3);
	//scene1->addObjectToWorld(Tri);
	scene1->addObjectToWorld(MyCam);
	scene1->addObjectToWorld(Light);
	//scene1->addObjectToWorld(Light2);
	//scene1->addObjectToWorld(Light3);
	//scene1->addObjectToWorld(Light4);
	//scene1->addObjectToWorld(Light5);
	//scene1->addObjectToWorld(Light6);
	
	//scene1->showObjectList();
	winMan.CurrentScene = scene1;
	winMan.RunCurrentScene();

	//scene2->addObjectToWorld(Yaz);
	//scene2->addObjectToWorld(Tri);
	scene2->addObjectToWorld(MyCam);
	scene2->addObjectToWorld(Light4);
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


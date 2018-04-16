#pragma once
#include "NL.hpp"
#include "TemplateScript.hpp"
#include "CamController.hpp"


int main() {
	//check_gl_error();

	//===========================================================================================
	//START!
	//===========================================================================================

	_NL::Engine::GameManager winMan("w1", 1024, 960, true, true, 60);
	_NL::Engine::WorldSpace* scene1 = new _NL::Engine::WorldSpace;
	
	//_NL::Engine::AudioSource* Audio = new _NL::Engine::AudioSource;
	//Audio->LoadAudioFile("deeplyMono.ogg");
	//Audio->Sound.play();

	//===========================================================================================
	//SKYBOXES
	//===========================================================================================

	_NL::Object::SkyboxObj* sky1 = new _NL::Object::SkyboxObj();
	_NL::Element::ShaderInstance* SkyboxDysplayShader = new _NL::Element::ShaderInstance("SkyboxDefaultVertshader.glsl", "SkyboxDefaultFragshader.glsl");
	_NL::Element::ShaderInstance* HDRImageShader = new _NL::Element::ShaderInstance("HDRImageDefaultVertshader.glsl", "HDRImageDefaultFragshader.glsl");
	_NL::Element::ShaderInstance* EnvironmentShader = new _NL::Element::ShaderInstance("EnvironmentDefaultVertshader.glsl", "EnvironmentDefaultFragshader.glsl");
	_NL::Element::ShaderInstance* PreFilterShader = new _NL::Element::ShaderInstance("PreFilterDefaultVertshader.glsl", "PreFilterDefaultFragshader.glsl");
	_NL::Element::ShaderInstance* BRDFshader = new _NL::Element::ShaderInstance("BRDFDefaultVertshader.glsl", "BRDFDefaultFragshader.glsl");
	
	
	sky1->SkyboxDysplayShader = SkyboxDysplayShader;
	sky1->HDRImageShader = HDRImageShader;
	sky1->EnvironmentShader = EnvironmentShader;
	sky1->PreFilterShader = PreFilterShader;
	sky1->BRDFShader = BRDFshader;

	sky1->createEnvironment("hdr1/spacehdri.jpg", 1024 * 2);
	sky1->createSkybox("hdr1/spacehdri.jpg", 1024*2);

	scene1->Skybox = sky1;
	
	//sky1->createSkybox(
	//	"sky2/ft.tga", 
	//	"sky2/bk.tga",
	//	"sky2/up.tga",
	//	"sky2/dn.tga", 
	//	"sky2/lf.tga", 
	//	"sky2/rt.tga"
	//);

	//sky1->createSkybox(
	//	"sky1/fadeaway_ft.tga", 
	//	"sky1/fadeaway_bk.tga", 
	//	"sky1/fadeaway_up.tga", 
	//	"sky1/fadeaway_dn.tga", 
	//	"sky1/fadeaway_lf.tga", 
	//	"sky1/fadeaway_rt.tga"
	//);

	//===========================================================================================
	//CAMERAS
	//===========================================================================================
	_NL::Object::CameraObj* MyCam2 = new _NL::Object::CameraObj("MyCam2", winMan.window->getSize().x / 2, winMan.window->getSize().y, winMan.window->getSize().x / 2, 0, 90, 0.1, 500, .5);
	
	_NL::Object::CameraObj* MyCam = new _NL::Object::CameraObj("MyCam", winMan.window->getSize().x, winMan.window->getSize().y, 0, 0, 90, 0.1, 500, 1, 2, 2, true, 10);
	_NL::Element::ShaderInstance* screenshader = new _NL::Element::ShaderInstance("screenQuadVshader.glsl", "screenQuadFshader.glsl");
	_NL::Element::ShaderInstance* GaussianBlur = new _NL::Element::ShaderInstance("GaussianBlurVshader.glsl", "GaussianBlurFshader.glsl");
	
	MyCam->addComponent(new _NL::Component::Script<CamController>);
	MyCam->getComponent<_NL::Component::Script<CamController>>()->CreateScript(new CamController());
	MyCam->getComponent<_NL::Component::Script<CamController>>()->getScript()->_this = MyCam;
	MyCam->getComponent<_NL::Component::Script<CamController>>()->getScript()->W = &winMan;
	MyCam->PostProcessingShader = screenshader;
	MyCam->PingPongShader = GaussianBlur;

	//===========================================================================================
	//MATERIAL SHADERS
	//===========================================================================================
	_NL::Element::ShaderInstance* defaultshader = new _NL::Element::ShaderInstance("defaultvertexshader.glsl", "defaultfragmentshader.glsl");
	_NL::Element::ShaderInstance* trishade = new _NL::Element::ShaderInstance("defaultvertexshader.glsl", "blueFrag.glsl");
	
	//===========================================================================================
	//OBJECTS 
	//===========================================================================================
	
	//(1)===========================================================================================
	_NL::Element::MaterialInstance* Mat1 = new _NL::Element::MaterialInstance();
	Mat1->AddNew_Material();
	Mat1->LoadAlbedoMap				("MyTexs/A.png",	0);
	Mat1->LoadRoughnessMap			("MyTexs/R.png",	0);
	Mat1->LoadMetalnessMap			("MyTexs/M.png",	0);
	Mat1->LoadNormalMap				("MyTexs/N.png",	0);
	Mat1->LoadAmbientOcclusionMap	("MyTexs/AO.png",	0);

	_NL::Object::GameObject* Sphere = new _NL::Object::GameObject("Shpere");
	Sphere->addComponent(new _NL::Component::Transform);
	Sphere->addComponent(new _NL::Component::MeshRenderer);
	Sphere->addComponent(new _NL::Component::Script<TemplateScript>);
	Sphere->getComponent<_NL::Component::MeshRenderer>()->Mesh = new _NL::Element::MeshInstance("Sphere.obj");
	Sphere->getComponent<_NL::Component::MeshRenderer>()->Shader = defaultshader;
	Sphere->getComponent<_NL::Component::MeshRenderer>()->Material = Mat1;
	Sphere->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->_this = Sphere;
	Sphere->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->W = &winMan;

	//(2)===========================================================================================
	_NL::Element::MaterialInstance* PBRGunMAT = new _NL::Element::MaterialInstance();
	PBRGunMAT->AddNew_Material();
	PBRGunMAT->LoadAlbedoMap			("Cerberus/Textures/Cerberus_A.tga", 0);
	PBRGunMAT->LoadRoughnessMap			("Cerberus/Textures/Cerberus_R.tga", 0);
	PBRGunMAT->LoadMetalnessMap			("Cerberus/Textures/Cerberus_M.tga", 0);
	PBRGunMAT->LoadNormalMap			("Cerberus/Textures/Cerberus_N.tga", 0);
	PBRGunMAT->LoadAmbientOcclusionMap	("Cerberus/Textures/Raw/Cerberus_AO.tga", 0);

	_NL::Object::GameObject* PBRGun = new _NL::Object::GameObject("PBRGun");
	PBRGun->addComponent(new _NL::Component::Transform);
	PBRGun->addComponent(new _NL::Component::MeshRenderer);
	PBRGun->addComponent(new _NL::Component::Script<TemplateScript>);
	PBRGun->getComponent<_NL::Component::MeshRenderer>()->Mesh = new _NL::Element::MeshInstance("Cerberus/PBRGun.obj");
	PBRGun->getComponent<_NL::Component::MeshRenderer>()->Shader = defaultshader;
	PBRGun->getComponent<_NL::Component::MeshRenderer>()->Material = PBRGunMAT;
	PBRGun->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->_this = PBRGun;
	PBRGun->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->W = &winMan;

	//(3)===========================================================================================
	_NL::Object::GameObject* Cube = new _NL::Object::GameObject("cube");
	Cube->addComponent(new _NL::Component::Transform);
	Cube->addComponent(new _NL::Component::MeshRenderer);
	Cube->addComponent(new _NL::Component::Script<TemplateScript>);
	Cube->getComponent<_NL::Component::MeshRenderer>()->Mesh = new _NL::Element::MeshInstance("Cube.obj");
	Cube->getComponent<_NL::Component::MeshRenderer>()->Shader = new _NL::Element::ShaderInstance("defaultvertexshader.glsl", "blueFrag.glsl");
	Cube->getComponent<_NL::Component::MeshRenderer>()->Material = PBRGunMAT;
	Cube->getComponent<_NL::Component::Script<TemplateScript>>()->CreateScript(new TemplateScript());
	Cube->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->_this = Cube;
	Cube->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->W = &winMan;

	//(4)===========================================================================================
	_NL::Object::GameObject* Quad = new _NL::Object::GameObject("Quad");
	Quad->addComponent(new _NL::Component::Transform());
	Quad->addComponent(new _NL::Component::MeshRenderer());
	Quad->addComponent(new _NL::Component::Script<TemplateScript>);
	Quad->getComponent<_NL::Component::MeshRenderer>()->Mesh = new _NL::Element::MeshInstance("quad.obj");
	Quad->getComponent<_NL::Component::MeshRenderer>()->Shader = defaultshader;
	Quad->getComponent<_NL::Component::MeshRenderer>()->Material = Mat1;
	Quad->getComponent<_NL::Component::Script<TemplateScript>>()->CreateScript(new TemplateScript());
	Quad->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->_this = Quad;
	Quad->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->W = &winMan;
	
	//===========================================================================================
	//LIGHTS
	//===========================================================================================

	_NL::Object::LightObject* Light = new _NL::Object::LightObject("Light");
	_NL::Object::LightObject* Light2 = new _NL::Object::LightObject("Light2");
	

	Sphere->getComponent<_NL::Component::Transform>()->transform.position.x = 150;
	Sphere->getComponent<_NL::Component::Transform>()->transform.position.y = 150;
	Sphere->getComponent<_NL::Component::Transform>()->transform.position.z = 150;
	Sphere->getComponent<_NL::Component::Transform>()->transform.scale *= 15;
	Sphere->getComponent<_NL::Component::Transform>()->transform.scale.y *= 2;

	Light->LightProperties.lightPosition.y = 150;
	Light->LightProperties.lightPosition.x = 100;
	Light->LightProperties.lightPosition.z = 150;
	Light->LightProperties.lightColor = glm::vec3(0, 2500, 2500);

	Light2->LightProperties.lightPosition.y = 8;
	Light2->LightProperties.lightPosition.x = 0;
	Light2->LightProperties.lightPosition.z = 0;
	Light2->LightProperties.lightColor = glm::vec3(150, 250, 350);

	Quad->getComponent<_NL::Component::Transform>()->transform.position.y -= 1;
	Quad->getComponent<_NL::Component::Transform>()->transform.scale *= 10;

	//===========================================================================================
	//SCENES 
	//===========================================================================================
	
	scene1->addObjectToWorld(Quad);
	scene1->addObjectToWorld(Sphere);
	scene1->addObjectToWorld(PBRGun);
	scene1->addObjectToWorld(Cube);
	PBRGun->getComponent<_NL::Component::Transform>()->EulerRotation(0, 180, 0);
	for(int i = 0; i< 100; i++){scene1->Instantiate(Cube, glm::vec3(rand() % 300, rand() % 300, rand() % 300), glm::vec3(rand() % 100+1, rand() % 100+1, rand() % 100+1), glm::vec3(rand() % 3 + 1, rand() % 3 + 1, rand() % 3 + 1));}

	scene1->addObjectToWorld(MyCam);
	scene1->addObjectToWorld(Light);
	scene1->addObjectToWorld(Light2);
	
	//scene1->showObjectList();
	winMan.CurrentScene = scene1;
	winMan.RunCurrentScene();

	return 0;
}


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

	sky1->createEnvironment("hdr1/lainPolar.png", 1024 * 2);
	//sky1->createSkybox("hdr1/lainPolar.png", 1024 * 2);
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
	
	_NL::Object::CameraObj* MyCam = new _NL::Object::CameraObj("MyCam", winMan.window->getSize().x/2, winMan.window->getSize().y, winMan.window->getSize().x/2, 0, 90, 0.1, 500, 1, 8, 2, true, 21);
	_NL::Object::CameraObj* MyCam2 = new _NL::Object::CameraObj("MyCam2", winMan.window->getSize().x/2, winMan.window->getSize().y, 0, 0, 90, 0.1, 500, .2, 0, 1, false);

	_NL::Element::ShaderInstance* screenshader = new _NL::Element::ShaderInstance("screenQuadVshader.glsl", "screenQuadFshader.glsl");
	_NL::Element::ShaderInstance* GaussianBlur = new _NL::Element::ShaderInstance("GaussianBlurVshader.glsl", "GaussianBlurFshader.glsl");

	MyCam->addComponent(new _NL::Component::Script<CamController>);
	MyCam->getComponent<_NL::Component::Script<CamController>>()->getScript()->_this = MyCam;
	MyCam->getComponent<_NL::Component::Script<CamController>>()->getScript()->W = &winMan;
	MyCam->PostProcessingShader = screenshader;
	MyCam->PingPongShader = GaussianBlur;
	MyCam->ClearScreenColor = glm::vec3(1, 0, 0);

	MyCam2->addComponent(new _NL::Component::Script<CamController>);
	MyCam2->getComponent<_NL::Component::Script<CamController>>()->getScript()->_this = MyCam2;
	MyCam2->getComponent<_NL::Component::Script<CamController>>()->getScript()->W = &winMan;
	MyCam2->PostProcessingShader = screenshader;
	MyCam2->PingPongShader = GaussianBlur;
	MyCam2->ClearScreenColor = glm::vec3(0, 1, 0);


	//===========================================================================================
	//CANVAS
	//===========================================================================================
	_NL::Element::ShaderInstance* UITexShder = new _NL::Element::ShaderInstance("UITexVertshader.glsl", "UITexFragshader.glsl");

	_NL::UI::UICanvas* Canvas1 = new _NL::UI::UICanvas();
	Canvas1->ImageRenderShader = UITexShder;

	_NL::Element::TextureInstance* crossairTex = new _NL::Element::TextureInstance("myTexs/nt.png", 1);
	_NL::Element::TextureInstance* bar = new _NL::Element::TextureInstance("myTexs/nt2.png", 1);
	_NL::Element::TextureInstance* bar2 = new _NL::Element::TextureInstance("myTexs/nt3.png", 1);
	_NL::Element::TextureInstance* laintrain = new _NL::Element::TextureInstance("myTexs/laiin.png", 1);
	
	_NL::UI::UIImage* ui1 = new _NL::UI::UIImage(crossairTex);
	ui1->scale.x *= 1;
	ui1->scale.y *= 1;
	ui1->AnchorPosition = glm::vec2(0, 0);
	ui1->PositionRelativeToAnchor = glm::vec2(winMan.window->getSize().x/2, winMan.window->getSize().y/2);
	ui1->Layer = 3;

	_NL::UI::UIImage* ui2 = new _NL::UI::UIImage(bar);
	ui2->widthHeight = glm::vec2(100,100);
	ui2->AnchorPosition = glm::vec2(10, 10);
	ui2->Layer = 2;

	_NL::UI::UIImage* ui3 = new _NL::UI::UIImage(bar2);
	ui3->widthHeight.x = winMan.window->getSize().x - 100;
	ui3->widthHeight.y *= 3;
	ui3->AnchorPosition = ui2->AnchorPosition;
	ui3->PositionRelativeToAnchor = glm::vec2(0, 50-ui3->widthHeight.y/2);
	ui3->Layer = 1;
	
	_NL::UI::UIImage* ui4 = new _NL::UI::UIImage(laintrain);
	ui4->Layer = 0;
	ui4->AnchorPosition = glm::vec2(100, 300);
	//ui4->widthHeight = glm::vec2(winMan.window->getSize().x, winMan.window->getSize().y);

	Canvas1->addUIElement(ui1);
	Canvas1->addUIElement(ui2);
	Canvas1->addUIElement(ui3);
	//Canvas1->addUIElement(ui4);

	//===========================================================================================
	//MATERIAL SHADERS
	//===========================================================================================
	_NL::Element::ShaderInstance* defaultshader = new _NL::Element::ShaderInstance("defaultvertexshader.glsl", "defaultfragmentshader.glsl");
	_NL::Element::ShaderInstance* trishade = new _NL::Element::ShaderInstance("defaultvertexshader.glsl", "simpleFrag.glsl");
	
	//===========================================================================================
	//OBJECTS 
	//===========================================================================================
	
	_NL::Element::TextureInstance* mtA = new _NL::Element::TextureInstance("MyTexs/A.png", 0);
	_NL::Element::TextureInstance* mtR = new  _NL::Element::TextureInstance("MyTexs/R2.png", 0);
	_NL::Element::TextureInstance* mtM = new _NL::Element::TextureInstance("MyTexs/M2.png", 0);
	_NL::Element::TextureInstance* mtN = new _NL::Element::TextureInstance("MyTexs/N.png", 0);
	_NL::Element::TextureInstance* mtAO = new _NL::Element::TextureInstance("MyTexs/AO.png", 0);

	_NL::Element::TextureInstance* CerberusA = new _NL::Element::TextureInstance("Cerberus/Textures/Cerberus_A.tga", 0);
	_NL::Element::TextureInstance* CerberusR = new  _NL::Element::TextureInstance("Cerberus/Textures/Cerberus_R.tga", 0);
	_NL::Element::TextureInstance* CerberusM = new _NL::Element::TextureInstance("Cerberus/Textures/Cerberus_M.tga", 0);
	_NL::Element::TextureInstance* CerberusN = new _NL::Element::TextureInstance("Cerberus/Textures/Cerberus_N.tga", 0);
	_NL::Element::TextureInstance* CerberusAO = new _NL::Element::TextureInstance("Cerberus/Textures/Raw/Cerberus_AO.tga", 0);

	//(1)===========================================================================================
	_NL::Element::MaterialInstance* Mat1 = new _NL::Element::MaterialInstance();
	Mat1->AddNew_Material();
	Mat1->LoadTexture	(_NL::Element::TEXTURE_TYPE::AlbedoMap, mtA, 0);
	Mat1->LoadTexture	(_NL::Element::TEXTURE_TYPE::RoughnessMap, mtR,	0);
	Mat1->LoadTexture	(_NL::Element::TEXTURE_TYPE::MetalnessMap, mtM,	0);
	Mat1->LoadTexture	(_NL::Element::TEXTURE_TYPE::NormalMap, mtN, 0);
	Mat1->LoadTexture	(_NL::Element::TEXTURE_TYPE::AmbientOcclusionMap, mtAO ,0);

	_NL::Object::GameObject* Sphere = new _NL::Object::GameObject("Shpere");
	Sphere->addComponent(new _NL::Component::Transform);
	Sphere->addComponent(new _NL::Component::MeshRenderer);
	Sphere->addComponent(new _NL::Component::Script<TemplateScript>);
	Sphere->getComponent<_NL::Component::MeshRenderer>()->Mesh = new _NL::Element::MeshInstance("Sphere.obj");
	Sphere->getComponent<_NL::Component::MeshRenderer>()->Shader = defaultshader;
	Sphere->getComponent<_NL::Component::MeshRenderer>()->Material = Mat1;
	Sphere->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->_this = Sphere;
	Sphere->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->W = &winMan;
	Sphere->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->target = &MyCam->Position;

	//(2)===========================================================================================
	_NL::Element::MaterialInstance* PBRGunMAT = new _NL::Element::MaterialInstance();
	PBRGunMAT->AddNew_Material();
	PBRGunMAT->LoadTexture(_NL::Element::TEXTURE_TYPE::AlbedoMap, CerberusA , 0);
	PBRGunMAT->LoadTexture(_NL::Element::TEXTURE_TYPE::RoughnessMap, CerberusR, 0);
	PBRGunMAT->LoadTexture(_NL::Element::TEXTURE_TYPE::MetalnessMap, CerberusM, 0);
	PBRGunMAT->LoadTexture(_NL::Element::TEXTURE_TYPE::NormalMap, CerberusN, 0);
	PBRGunMAT->LoadTexture(_NL::Element::TEXTURE_TYPE::AmbientOcclusionMap, CerberusAO, 0);

	_NL::Object::GameObject* PBRGun = new _NL::Object::GameObject("PBRGun");
	PBRGun->addComponent(new _NL::Component::Transform);
	PBRGun->addComponent(new _NL::Component::MeshRenderer);
	PBRGun->addComponent(new _NL::Component::Script<TemplateScript>);
	PBRGun->getComponent<_NL::Component::MeshRenderer>()->Mesh = new _NL::Element::MeshInstance("Cerberus/PBRGun.obj");
	PBRGun->getComponent<_NL::Component::MeshRenderer>()->Shader = defaultshader;
	PBRGun->getComponent<_NL::Component::MeshRenderer>()->Material = PBRGunMAT;
	PBRGun->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->_this = PBRGun;
	PBRGun->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->W = &winMan;
	PBRGun->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->target = &MyCam->Position;

	//(3)===========================================================================================
	_NL::Object::GameObject* Cube = new _NL::Object::GameObject("cube");
	Cube->addComponent(new _NL::Component::Transform);
	Cube->addComponent(new _NL::Component::MeshRenderer);
	Cube->addComponent(new _NL::Component::Script<TemplateScript>);
	Cube->getComponent<_NL::Component::MeshRenderer>()->Mesh = new _NL::Element::MeshInstance("Cube.obj");
	Cube->getComponent<_NL::Component::MeshRenderer>()->Shader = new _NL::Element::ShaderInstance("defaultvertexshader.glsl", "simpleFrag.glsl");
	Cube->getComponent<_NL::Component::MeshRenderer>()->Material = Mat1;
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
	Quad->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->_this = Quad;
	Quad->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->W = &winMan;
	Quad->getComponent<_NL::Component::Script<TemplateScript>>()->getScript()->target = &MyCam->Position;


	Quad->getComponent<_NL::Component::Transform>()->transform.position.y -= 1;
	Quad->getComponent<_NL::Component::Transform>()->transform.scale *= 10;

	//===========================================================================================
	//LIGHTS
	//===========================================================================================

	_NL::Object::LightObject* Light = new _NL::Object::LightObject("Light");

	_NL::Object::LightObject* Light2 = new _NL::Object::LightObject("Light2");
	Light2->addComponent(new _NL::Component::MeshRenderer);
	Light2->addComponent(new _NL::Component::Transform);
	Light2->getComponent<_NL::Component::MeshRenderer>()->Material = Mat1;
	Light2->getComponent<_NL::Component::MeshRenderer>()->Mesh = new _NL::Element::MeshInstance("Cube.obj");
	Light2->getComponent<_NL::Component::MeshRenderer>()->Shader = new _NL::Element::ShaderInstance("defaultvertexshader.glsl", "simpleFrag.glsl");

	Sphere->getComponent<_NL::Component::Transform>()->transform.position.x = 150;
	Sphere->getComponent<_NL::Component::Transform>()->transform.position.y = 150;
	Sphere->getComponent<_NL::Component::Transform>()->transform.position.z = 150;
	Sphere->getComponent<_NL::Component::Transform>()->transform.scale *= 15;
	Sphere->getComponent<_NL::Component::Transform>()->transform.scale.y *= 2;

	Light->LightProperties.lightPosition.y = 150;
	Light->LightProperties.lightPosition.x = 100;
	Light->LightProperties.lightPosition.z = 150;
	Light->LightProperties.lightColor = glm::vec3(2500, 2500, 2500);

	Light2->LightProperties.lightPosition.y = 8;
	Light2->LightProperties.lightPosition.x = 0;
	Light2->LightProperties.lightPosition.z = 0;
	Light2->LightProperties.lightColor = glm::vec3(150, 250, 350);
	Light2->getComponent<_NL::Component::Transform>()->transform.position = Light2->LightProperties.lightPosition;

	//===========================================================================================
	//SCENES 
	//===========================================================================================
	
	scene1->addObjectToWorld(Quad);
	scene1->addObjectToWorld(Sphere);
	scene1->addObjectToWorld(PBRGun);
	scene1->addObjectToWorld(Cube);
	scene1->addObjectToWorld(Canvas1);

	for(int i = 0; i< 100; i++){
		scene1->Instantiate(
		Cube, 
		glm::vec3(rand() % 300, rand() % 300, rand() % 300), 
		glm::quat(glm::vec3(rand() % 360, rand() % 360, rand() % 360)), 
		glm::vec3(rand() % 3 + 1, rand() % 3 + 1, rand() % 3 + 1)
		);
	}

	scene1->addObjectToWorld(MyCam);
	scene1->addObjectToWorld(MyCam2);
	
	scene1->addObjectToWorld(Light);
	scene1->addObjectToWorld(Light2);
	
	//scene1->showObjectList();
	winMan.CurrentScene = scene1;
	winMan.RunCurrentScene();

	return 0;
}


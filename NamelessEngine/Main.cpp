#pragma once
#include "NL.hpp"
#include "TestScript.hpp"
#include "CamController.hpp"
#include "ParticleScript.hpp"



int main() {
	//check_gl_error();

	//===========================================================================================
	//START!
	//===========================================================================================
	//XMLfileReader X;
	//X.Load("COLLADATEST/houseColladaAnim.dae",true,false);
	
	_NL::Engine::GameManager winMan("w1", 1024, 960, true, true, 120);

	//_NL::Element::ShaderInstance* DEPTHshader = new _NL::Element::ShaderInstance("DepthPassVertshader.glsl", "DepthPassFragshader.glsl");
	//winMan.DepthPassShader = DEPTHshader;

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

	sky1->createEnvironment("myTexs/A.png", 1024 * 2);
	//sky1->createSkybox("hdr1/japanhdri (4).jpg", 1024 * 2);
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
	
	_NL::Object::CameraObj* MyCam = new _NL::Object::CameraObj("MyCam", winMan.window->getSize().x, winMan.window->getSize().y, 0, 0, 90, 0.1, 7000, 1, 16, 2, true, 21);
	_NL::Object::CameraObj* MyCam2 = new _NL::Object::CameraObj("MyCam2", winMan.window->getSize().x, winMan.window->getSize().y/2, 0, winMan.window->getSize().y / 2, 90, 0.1, 500, 1, 0, 1, false);

	_NL::Element::ShaderInstance* screenshader = new _NL::Element::ShaderInstance("screenQuadVshader.glsl", "screenQuadFshader.glsl");
	_NL::Element::ShaderInstance* GaussianBlur = new _NL::Element::ShaderInstance("GaussianBlurVshader.glsl", "GaussianBlurFshader.glsl");

	MyCam->addComponent(new _NL::Component::CppScript<CamController>);
	MyCam->getComponent<_NL::Component::CppScript<CamController>>()->getScript()->_this = MyCam;
	MyCam->getComponent<_NL::Component::CppScript<CamController>>()->getScript()->W = &winMan;
	MyCam->PostProcessingShader = screenshader;
	MyCam->PingPongShader = GaussianBlur;
	MyCam->ClearScreenColor = glm::vec3(1, 0, 0);

	MyCam2->addComponent(new _NL::Component::CppScript<CamController>);
	MyCam2->getComponent<_NL::Component::CppScript<CamController>>()->getScript()->_this = MyCam2;
	MyCam2->getComponent<_NL::Component::CppScript<CamController>>()->getScript()->W = &winMan;
	MyCam2->PostProcessingShader = screenshader;
	MyCam2->PingPongShader = GaussianBlur;
	MyCam2->ClearScreenColor = glm::vec3(0, 1, 0);

	
	//===========================================================================================
	//CANVAS
	//===========================================================================================
	_NL::Element::ShaderInstance* UITexShder = new _NL::Element::ShaderInstance("UITexVertshader.glsl", "UITexFragshader.glsl");

	_NL::UI::UICanvas* Canvas1 = new _NL::UI::UICanvas(winMan.window);
	Canvas1->ImageRenderShader = UITexShder;

	_NL::Element::TextureInstance* crossairTex = new _NL::Element::TextureInstance("myTexs/nt.png", 1);
	_NL::Element::TextureInstance* bar = new _NL::Element::TextureInstance("myTexs/nt2.png", 1);
	_NL::Element::TextureInstance* bar2 = new _NL::Element::TextureInstance("myTexs/nt3.png", 1);
	
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

	_NL::UI::UIText* ut1 = new _NL::UI::UIText("fonts/Consolas.ttf", "Hello World ! ^_^");
	ut1->AnchorPosition = glm::vec2(100, 100);
	ut1->Layer = 10;

	Canvas1->addUIElement(ut1);
	//Canvas1->addUIElement(ui1);
	//Canvas1->addUIElement(ui2);
	//Canvas1->addUIElement(ui3);

	//===========================================================================================
	//MATERIAL SHADERS
	//===========================================================================================
	_NL::Element::ShaderInstance* defaultshader = new _NL::Element::ShaderInstance("defaultvertexshader.glsl", "defaultfragmentshader.glsl");
	_NL::Element::ShaderInstance* simpleshade = new _NL::Element::ShaderInstance("defaultvertexshader.glsl", "simpleFrag.glsl");
	
	//===========================================================================================
	//OBJECTS 
	//===========================================================================================
	

	_NL::Element::MaterialInstance* Mat1 = new _NL::Element::MaterialInstance();
	Mat1->AddNew_Material();
	_NL::Element::TextureInstance* mtA = new _NL::Element::TextureInstance("MyTexs/fire.png", 0);
	_NL::Element::TextureInstance* mtR = new  _NL::Element::TextureInstance("MyTexs/R2.png", 0);
	_NL::Element::TextureInstance* mtM = new _NL::Element::TextureInstance("MyTexs/M.png", 0);
	_NL::Element::TextureInstance* mtN = new _NL::Element::TextureInstance("MyTexs/N.png", 0);
	_NL::Element::TextureInstance* mtAO = new _NL::Element::TextureInstance("MyTexs/AO.png", 0);
	Mat1->LoadTexture(_NL::Element::TEXTURE_TYPE::AlbedoMap, mtA, 0);
	Mat1->LoadTexture(_NL::Element::TEXTURE_TYPE::RoughnessMap, mtR, 0);
	Mat1->LoadTexture(_NL::Element::TEXTURE_TYPE::MetalnessMap, mtM, 0);
	Mat1->LoadTexture(_NL::Element::TEXTURE_TYPE::NormalMap, mtN, 0);
	Mat1->LoadTexture(_NL::Element::TEXTURE_TYPE::AmbientOcclusionMap, mtAO, 0);


	_NL::Element::MaterialInstance* PBRGunMAT = new _NL::Element::MaterialInstance();
	PBRGunMAT->AddNew_Material();
	_NL::Element::TextureInstance* CerberusA = new _NL::Element::TextureInstance("Cerberus/Textures/Cerberus_A.tga", 0);
	_NL::Element::TextureInstance* CerberusR = new  _NL::Element::TextureInstance("Cerberus/Textures/Cerberus_R.tga", 0);
	_NL::Element::TextureInstance* CerberusM = new _NL::Element::TextureInstance("Cerberus/Textures/Cerberus_M.tga", 0);
	_NL::Element::TextureInstance* CerberusN = new _NL::Element::TextureInstance("Cerberus/Textures/Cerberus_N.tga", 0);
	_NL::Element::TextureInstance* CerberusAO = new _NL::Element::TextureInstance("Cerberus/Textures/Raw/Cerberus_AO.tga", 0);
	PBRGunMAT->LoadTexture(_NL::Element::TEXTURE_TYPE::AlbedoMap, CerberusA, 0);
	PBRGunMAT->LoadTexture(_NL::Element::TEXTURE_TYPE::RoughnessMap, CerberusR, 0);
	PBRGunMAT->LoadTexture(_NL::Element::TEXTURE_TYPE::MetalnessMap, CerberusM, 0);
	PBRGunMAT->LoadTexture(_NL::Element::TEXTURE_TYPE::NormalMap, CerberusN, 0);
	PBRGunMAT->LoadTexture(_NL::Element::TEXTURE_TYPE::AmbientOcclusionMap, CerberusAO, 0);


	//(1)===========================================================================================


	_NL::Object::GameObject* HouseCollada = new _NL::Object::GameObject("HouseCollada");
	HouseCollada->addComponent(new _NL::Component::Transform);
	HouseCollada->addComponent(new _NL::Component::MeshRenderer);
	HouseCollada->addComponent(new _NL::Component::CppScript<TestScript>);
	HouseCollada->getComponent<_NL::Component::MeshRenderer>()->Mesh = new _NL::Element::MeshInstance("COLLADATEST/colladaScene.obj");
	HouseCollada->getComponent<_NL::Component::MeshRenderer>()->Shader = simpleshade;
	HouseCollada->getComponent<_NL::Component::MeshRenderer>()->Material = PBRGunMAT;
	HouseCollada->getComponent<_NL::Component::CppScript<TestScript>>()->getScript()->_this = HouseCollada;
	HouseCollada->getComponent<_NL::Component::CppScript<TestScript>>()->getScript()->W = &winMan;

	HouseCollada->getComponent<_NL::Component::Transform>()->transform.position.x = 0;
	HouseCollada->getComponent<_NL::Component::Transform>()->transform.position.y = 0;
	HouseCollada->getComponent<_NL::Component::Transform>()->transform.position.z = 0;
	//HouseCollada->getComponent<_NL::Component::Transform>()->transform.scale *= -1;

	////(2)===========================================================================================
	//
	//_NL::Object::GameObject* PBRGun = new _NL::Object::GameObject("PBRGun");
	//PBRGun->addComponent(new _NL::Component::Transform);
	//PBRGun->addComponent(new _NL::Component::MeshRenderer);
	//PBRGun->addComponent(new _NL::Component::CppScript<TestScript>);
	//PBRGun->getComponent<_NL::Component::MeshRenderer>()->Mesh = new _NL::Element::MeshInstance("Cerberus/PBRGun.obj");
	//PBRGun->getComponent<_NL::Component::MeshRenderer>()->Shader = defaultshader;
	//PBRGun->getComponent<_NL::Component::MeshRenderer>()->Material = PBRGunMAT;
	//PBRGun->getComponent< _NL::Component::CppScript<TestScript>>()->getScript()->_this = PBRGun;
	//PBRGun->getComponent< _NL::Component::CppScript<TestScript>>()->getScript()->W = &winMan;
	//
	////(3)===========================================================================================
	//_NL::Element::MeshInstance* Cubemesh = new _NL::Element::MeshInstance("Cube.obj");
	//
	//_NL::Object::GameObject* Cube = new _NL::Object::GameObject("cube1");
	//Cube->addComponent(new _NL::Component::Transform);
	//Cube->addComponent(new _NL::Component::MeshRenderer);
	//Cube->addComponent(new _NL::Component::CppScript<TestScript>);
	//Cube->getComponent<_NL::Component::MeshRenderer>()->Mesh = Cubemesh;
	//Cube->getComponent<_NL::Component::MeshRenderer>()->Shader = simpleshade;
	//Cube->getComponent<_NL::Component::MeshRenderer>()->Material = Mat1;
	//Cube->getComponent< _NL::Component::CppScript<TestScript>>()->getScript()->_this = Cube;
	//Cube->getComponent< _NL::Component::CppScript<TestScript>>()->getScript()->W = &winMan;
	//
	//_NL::Object::GameObject* Cube2 = new _NL::Object::GameObject("cube2");
	//Cube2->addComponent(new _NL::Component::Transform);
	//Cube2->addComponent(new _NL::Component::MeshRenderer);
	//Cube2->getComponent<_NL::Component::MeshRenderer>()->Mesh = Cubemesh;
	//Cube2->getComponent<_NL::Component::MeshRenderer>()->Shader = simpleshade;
	//Cube2->getComponent<_NL::Component::MeshRenderer>()->Material = Mat1;
	//
	//_NL::Object::GameObject* Cube3 = new _NL::Object::GameObject("cube3");
	//Cube3->addComponent(new _NL::Component::Transform);
	//Cube3->addComponent(new _NL::Component::MeshRenderer);
	//Cube3->getComponent<_NL::Component::MeshRenderer>()->Mesh = Cubemesh;
	//Cube3->getComponent<_NL::Component::MeshRenderer>()->Shader = simpleshade;
	//Cube3->getComponent<_NL::Component::MeshRenderer>()->Material = Mat1;
	//
	//Cube2->getComponent<_NL::Component::Transform>()->transform.position = glm::vec3(3, 0, -3);
	//Cube2->getComponent<_NL::Component::Transform>()->transform.scale = glm::vec3(1, 5, 1);
	//Cube2->Parent = Cube;
	//
	//Cube3->getComponent<_NL::Component::Transform>()->transform.position = glm::vec3(3, 0, 3);
	//Cube3->getComponent<_NL::Component::Transform>()->transform.scale = glm::vec3(1, 5, 1);
	//Cube3->Parent = Cube;
	//
	////(4)===========================================================================================
	_NL::Object::GameObject* Quad = new _NL::Object::GameObject("Quad");
	Quad->addComponent(new _NL::Component::Transform());
	Quad->addComponent(new _NL::Component::MeshRenderer());
	Quad->addComponent(new _NL::Component::CppScript<TestScript>);
	Quad->getComponent<_NL::Component::MeshRenderer>()->Mesh = new _NL::Element::MeshInstance("quad.obj");
	Quad->getComponent<_NL::Component::MeshRenderer>()->Shader = defaultshader;
	Quad->getComponent<_NL::Component::MeshRenderer>()->Material = PBRGunMAT;
	Quad->getComponent< _NL::Component::CppScript<TestScript>>()->getScript()->_this = Quad;
	Quad->getComponent< _NL::Component::CppScript<TestScript>>()->getScript()->W = &winMan;
	
	Quad->getComponent<_NL::Component::Transform>()->transform.position.y -= 1;
	Quad->getComponent<_NL::Component::Transform>()->transform.scale *= 100;



	//===========================================================================================
	//LIGHTS
	//===========================================================================================

	_NL::Object::LightObject* Light1 = new _NL::Object::LightObject("Light1");
	Light1->addComponent(new _NL::Component::CppScript<TestScript>);
	Light1->getComponent<_NL::Component::CppScript<TestScript>>()->getScript()->_this = Light1;
	Light1->getComponent<_NL::Component::CppScript<TestScript>>()->getScript()->W = &winMan;
	//Light1->addComponent(new _NL::Component::MeshRenderer);
	Light1->addComponent(new _NL::Component::Transform);
	//Light1->getComponent<_NL::Component::MeshRenderer>()->Material = Mat1;
	//Light1->getComponent<_NL::Component::MeshRenderer>()->Mesh = Cubemesh;
	//Light1->getComponent<_NL::Component::MeshRenderer>()->Shader = new _NL::Element::ShaderInstance("defaultvertexshader.glsl", "simpleFrag.glsl");
	

	Light1->getComponent<_NL::Component::Transform>()->
		transform.position = glm::vec3(0, 10, 0);

	Light1->LightProperties.lightPosition = Light1->getComponent<_NL::Component::Transform>()->transform.position;
	
	Light1->LightProperties.lightColor = glm::vec3(1000, 1000, 1000);
	
	//Light1->LightProperties.lightDirection = glm::vec3(0, -1, 0);

	//Light1->LightProperties.lightSpotAngle = 12.5f;

	//===========================================================================================
	//PARTICLE SYSTEMS
	//===========================================================================================


	_NL::Object::ParticleObj* flameParticle = new _NL::Object::ParticleObj();
	
	_NL::Element::TextureInstance* FlameAlbedo = new _NL::Element::TextureInstance("MyTexs/fire.png", false);
	
	_NL::Element::MaterialInstance* flameMat = new _NL::Element::MaterialInstance();
	flameMat->AddNew_Material();
	flameMat->LoadTexture(_NL::Element::AlbedoMap, FlameAlbedo, 0);
	
	flameParticle->addComponent(new _NL::Component::MeshRenderer);
	flameParticle->addComponent(new _NL::Component::Transform);
	flameParticle->getComponent<_NL::Component::MeshRenderer>()->Mesh = new _NL::Element::MeshInstance("quad.obj");
	flameParticle->getComponent<_NL::Component::MeshRenderer>()->Shader = simpleshade;
	flameParticle->getComponent<_NL::Component::MeshRenderer>()->Material = flameMat;
	flameParticle->getComponent<_NL::Component::MeshRenderer>()->initGLObj();
	flameParticle->lifeTime = 1;
	
	_NL::Object::ParticleSystem* PS1 = new _NL::Object::ParticleSystem();
	_NL::Component::CppScript<ParticleScript>* Pbehaviour = new _NL::Component::CppScript<ParticleScript>();
	Pbehaviour->getScript()->W = &winMan;
	PS1->Particle = flameParticle;
	PS1->ParticlesBehavior = Pbehaviour->getScript();
	PS1->SpawnerTransform.SpawnerShape = _NL::Object::ParticleSystem::SPHERE;
	PS1->SpawnerTransform.Position.y = 10;
	PS1->SpawnerTransform.Scale *= 0.1f;
	PS1->SpawnerTransform.SpawnerHeightY = 10;
	PS1->SpawnerTransform.SpawnerWidthX = 10;
	PS1->SpawnerTransform.SpawnerWidthZ = 10;
	PS1->SpawnerTransform.SpawnerConeVertexRadius = 0.0f;
	PS1->TimeScale = &winMan.GameTime;
	PS1->SpawnPerFrame = 1;
	scene1->addObjectToWorld(PS1);


	//===========================================================================================
	//SCENES 
	//===========================================================================================
	
	scene1->addObjectToWorld(Quad);
	scene1->addObjectToWorld(HouseCollada);
	//scene1->addObjectToWorld(PBRGun);
	//scene1->addObjectToWorld(Cube);
	//scene1->addObjectToWorld(Cube2);
	//scene1->addObjectToWorld(Cube3);
	scene1->addObjectToWorld(Canvas1);

	//for(int i = 0; i< 500; i++){
	//	scene1->Instantiate(
	//	Cube, 
	//	glm::vec3(rand() % 300, rand() % 300, rand() % 300), 
	//	glm::quat(glm::vec3(rand() % 360, rand() % 360, rand() % 360)), 
	//	glm::vec3(rand() % 3 + 1, rand() % 3 + 1, rand() % 3 + 1)
	//	);
	//}
	
	scene1->addObjectToWorld(MyCam);
	//scene1->addObjectToWorld(MyCam2);
	
	scene1->addObjectToWorld(Light1);
	//scene1->addObjectToWorld(Light2);
	
	//scene1->showObjectList();
	winMan.RunScene(scene1);
	return 0;
}


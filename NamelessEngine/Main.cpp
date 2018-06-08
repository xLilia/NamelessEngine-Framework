#pragma once
#include "NL.hpp"
#include "TestScript.hpp"
#include "CamController.hpp"
#include "ParticleScript.hpp"
#include "GausianBlurPostProcessingEffect.hpp"


int main() {
	//check_gl_error();

	//===========================================================================================
	//ENGINE NLManager SCENE 
	//===========================================================================================
	
	_NL::Engine::NLManager winMan("DEMO SCENE ALPHA 0.6", 1024, 768, true, true, false, false, false);
	_NL::Engine::WorldSpace* scene1 = new _NL::Engine::WorldSpace;

	//===========================================================================================
	//AUDIO: DEPRECATED ...
	//===========================================================================================

	//_NL::Engine::AudioSource* Audio = new _NL::Engine::AudioSource;
	//Audio->LoadAudioFile("deeplyMono.ogg");
	//Audio->Sound.play();

	//===========================================================================================
	//SKYBOXES: STABLE !
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

	sky1->createEnvironment("myhdri/japanhdri (7).jpg");
	sky1->createSkybox("myhdri/japanhdri (7).jpg");
	scene1->Skybox = sky1;
	
	//===========================================================================================
	//MULTIPLE CAMERAS / VIEWPORTS: STABLE !
	//===========================================================================================
	
	_NL::Object::CameraObj* MyCam = new _NL::Object::CameraObj("MyCam", winMan.window->getSize().x, winMan.window->getSize().y, 0, 0, 90, 0.1, 500, 1.0, 10, GL_LINEAR);
	_NL::Object::CameraObj* MyCam2 = new _NL::Object::CameraObj("MyCam2", winMan.window->getSize().x / 4, winMan.window->getSize().y / 4, 0, 0, 90, 0.1, 500, 1, 10, GL_LINEAR);
	_NL::Object::CameraObj* MyCam3 = new _NL::Object::CameraObj("MyCam3", winMan.window->getSize().x/4, winMan.window->getSize().y/4, winMan.window->getSize().x - winMan.window->getSize().x / 4, 0, 90, 0.1, 500, .3, 10, GL_NEAREST);
	
	_NL::Element::ShaderInstance* screenshader = new _NL::Element::ShaderInstance("DdeferedScreenQuadvertexshader.glsl", "DdeferedScreenQuadfragmentshader.glsl");
	_NL::Element::ShaderInstance* GaussianBlur = new _NL::Element::ShaderInstance("GaussianBlurVshader.glsl", "GaussianBlurFshader.glsl");
	
	GausianBlurPostProcessingEffect* GausianBlur = new GausianBlurPostProcessingEffect();
	GausianBlur->PingPongIterations = 2;
	GausianBlur->PingPongShader = GaussianBlur;
	GausianBlur->TargetCam = MyCam2;

	MyCam->addComponent(new _NL::Component::CppScript<CamController>);
	MyCam->getComponent<_NL::Component::CppScript<CamController>>()->getScript()->W = &winMan;
	MyCam->FinalPassShader = screenshader;
	MyCam->GenerateFrameBuffers();
	scene1->addObjectToWorld(MyCam);

	MyCam2->addComponent(new _NL::Component::CppScript<CamController>);
	MyCam2->getComponent<_NL::Component::CppScript<CamController>>()->getScript()->W = &winMan;
	MyCam2->FinalPassShader = screenshader;
	MyCam2->GenerateFrameBuffers();
	MyCam2->PostProcessingStack.push_back(GausianBlur);
	scene1->addObjectToWorld(MyCam2);

	MyCam3->addComponent(new _NL::Component::CppScript<CamController>);
	MyCam3->getComponent<_NL::Component::CppScript<CamController>>()->getScript()->W = &winMan;
	MyCam3->FinalPassShader = screenshader;
	MyCam3->GenerateFrameBuffers();
	scene1->addObjectToWorld(MyCam3);


	//===========================================================================================
	//2D CANVAS: 2D IMAGE RENDER - STABLE ! - TEXT RENDER - UNSTABLE... 
	//===========================================================================================
	
	_NL::Element::ShaderInstance* UITexShder = new _NL::Element::ShaderInstance("UITexVertshader.glsl", "UITexFragshader.glsl");
	_NL::UI::UICanvas* Canvas1 = new _NL::UI::UICanvas(winMan.window);
	Canvas1->ImageRenderShader = UITexShder;

	_NL::Element::TextureInstance* crossairTex = new _NL::Element::TextureInstance("myTexs/nt.png", 1);
	_NL::Element::TextureInstance* bar = new _NL::Element::TextureInstance("myTexs/nt2.png", 1);
	_NL::Element::TextureInstance* cross = new _NL::Element::TextureInstance("myTexs/nt3.png", 1);
	
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

	_NL::UI::UIImage* ui3 = new _NL::UI::UIImage(cross);
	ui3->widthHeight.x = winMan.window->getSize().x - 100;
	ui3->widthHeight.y *= 3;
	ui3->AnchorPosition = ui2->AnchorPosition;
	ui3->PositionRelativeToAnchor = glm::vec2(0, 50-ui3->widthHeight.y/2);
	ui3->Layer = 1;

	_NL::UI::UIText* ut1 = new _NL::UI::UIText("Myfonts/Consolas.ttf", "Hello World ! \(^_^)/ 12345");
	ut1->AnchorPosition = glm::vec2(100, 100);
	ut1->Layer = 4;

	Canvas1->addUIElement(ut1);
	Canvas1->addUIElement(ui1); 
	Canvas1->addUIElement(ui2);	
	Canvas1->addUIElement(ui3);	

	scene1->addObjectToWorld(Canvas1);

	//===========================================================================================
	//MATERIAL SHADERS - STABLE !
	//===========================================================================================
	_NL::Element::ShaderInstance* defaultshader = new _NL::Element::ShaderInstance("Ddeferedvertexshader.glsl", "Ddeferedfragmentshader.glsl");
	_NL::Element::ShaderInstance* simpleshade = new _NL::Element::ShaderInstance("Ddeferedvertexshader.glsl", "CustomShader.glsl");
	

	_NL::Element::MaterialInstance* Mat1 = new _NL::Element::MaterialInstance();
	_NL::Element::TextureInstance* mtA = new _NL::Element::TextureInstance("MyTexs/cyber-ground-emisiive.jpg", 0);
	_NL::Element::TextureInstance* mtR = new  _NL::Element::TextureInstance("MyTexs/R2.png", 0);
	_NL::Element::TextureInstance* mtM = new _NL::Element::TextureInstance("MyTexs/cyber-ground-emisiive.jpg", 0);
	_NL::Element::TextureInstance* mtN = new _NL::Element::TextureInstance("MyTexs/N.png", 0);
	_NL::Element::TextureInstance* mtAO = new _NL::Element::TextureInstance("MyTexs/AO.png", 0);
	Mat1->LoadTexture(_NL::Element::TEXTURE_TYPE::AlbedoMap, mtA, 0);
	Mat1->LoadTexture(_NL::Element::TEXTURE_TYPE::RoughnessMap, mtR, 0);
	Mat1->LoadTexture(_NL::Element::TEXTURE_TYPE::MetalnessMap, mtM, 0);
	Mat1->LoadTexture(_NL::Element::TEXTURE_TYPE::NormalMap, mtN, 0);
	Mat1->LoadTexture(_NL::Element::TEXTURE_TYPE::AmbientOcclusionMap, mtAO, 0);


	_NL::Element::MaterialInstance* PBRGunMAT = new _NL::Element::MaterialInstance();
	_NL::Element::TextureInstance* CerberusA = new _NL::Element::TextureInstance("MyTexs/Cerberus/Cerberus_A.tga", 0);
	_NL::Element::TextureInstance* CerberusR = new  _NL::Element::TextureInstance("MyTexs/Cerberus/Cerberus_R.tga", 0);
	_NL::Element::TextureInstance* CerberusM = new _NL::Element::TextureInstance("MyTexs/Cerberus/Cerberus_M.tga", 0);
	_NL::Element::TextureInstance* CerberusN = new _NL::Element::TextureInstance("MyTexs/Cerberus/Cerberus_N.tga", 0);
	_NL::Element::TextureInstance* CerberusAO = new _NL::Element::TextureInstance("MyTexs/Cerberus/Raw/Cerberus_AO.tga", 0);
	PBRGunMAT->LoadTexture(_NL::Element::TEXTURE_TYPE::AlbedoMap, CerberusA, 0);
	PBRGunMAT->LoadTexture(_NL::Element::TEXTURE_TYPE::RoughnessMap, CerberusR, 0);
	PBRGunMAT->LoadTexture(_NL::Element::TEXTURE_TYPE::MetalnessMap, CerberusM, 0);
	PBRGunMAT->LoadTexture(_NL::Element::TEXTURE_TYPE::NormalMap, CerberusN, 0);
	PBRGunMAT->LoadTexture(_NL::Element::TEXTURE_TYPE::AmbientOcclusionMap, CerberusAO, 0);

	//===========================================================================================
	//OBJECTS - STABLE !
	//===========================================================================================

	//(1)===========================================================================================

	_NL::Object::GameObject* PBRGun = new _NL::Object::GameObject("pbrGUN");
	PBRGun->addComponent(new _NL::Component::Transform);
	PBRGun->addComponent(new _NL::Component::MeshRenderer);
	PBRGun->addComponent(new _NL::Component::CppScript<TestScript>);
	PBRGun->getComponent<_NL::Component::MeshRenderer>()->Mesh = new _NL::Element::MeshInstance("Mymodels/Cerberus/pbrgun.obj");
	PBRGun->getComponent<_NL::Component::MeshRenderer>()->Shader = defaultshader;
	PBRGun->getComponent<_NL::Component::MeshRenderer>()->Material = PBRGunMAT;
	PBRGun->getComponent<_NL::Component::CppScript<TestScript>>()->getScript()->W = &winMan;

	PBRGun->getComponent<_NL::Component::Transform>()->transform.position.x = 0;
	PBRGun->getComponent<_NL::Component::Transform>()->transform.position.y = 5;
	PBRGun->getComponent<_NL::Component::Transform>()->transform.position.z = 0;
	
	PBRGun->getComponent<_NL::Component::Transform>()->transform.scale *= 3;


	scene1->addObjectToWorld(PBRGun);

	//(2)===========================================================================================
	
	_NL::Element::MeshInstance* quadmesh = new _NL::Element::MeshInstance("MyModels/quad.obj");
	_NL::Object::GameObject* Quad = new _NL::Object::GameObject("Quad");
	Quad->addComponent(new _NL::Component::Transform());
	Quad->addComponent(new _NL::Component::MeshRenderer());
	Quad->addComponent(new _NL::Component::CppScript<TestScript>);
	Quad->getComponent<_NL::Component::MeshRenderer>()->Mesh = quadmesh;
	Quad->getComponent<_NL::Component::MeshRenderer>()->Shader = simpleshade;
	Quad->getComponent<_NL::Component::MeshRenderer>()->Material = Mat1;
	Quad->getComponent<_NL::Component::MeshRenderer>()->GL_CullFace = false;
	Quad->getComponent<_NL::Component::CppScript<TestScript>>()->getScript()->W = &winMan;

	Quad->getComponent<_NL::Component::Transform>()->transform.position.y -= 1;
	Quad->getComponent<_NL::Component::Transform>()->transform.scale *= 100;
	
	scene1->addObjectToWorld(Quad);

	//===========================================================================================
	//LIGHTS - STABLE !
	//===========================================================================================

	_NL::Object::LightObject* Light1 = new _NL::Object::LightObject("Light1");
	Light1->LightProperties.setDirectionalLightProperties(glm::vec3(.5, 1.5, .5), glm::vec3(0, 0, 0), glm::vec3(0,-1,0));

	_NL::Object::LightObject* Light2 = new _NL::Object::LightObject("Light2");
	Light2->LightProperties.setSpotLightProperties(glm::vec3(10000, 5000, 2500), glm::vec3(15, 25, 0), glm::vec3(-1, -1, 0), 12, 70);
	
	_NL::Object::LightObject* Light3 = new _NL::Object::LightObject("Light3");
	Light3->LightProperties.setPointLightProperties(glm::vec3(100, 100, 1000), glm::vec3(75, 5, 75));

	scene1->addObjectToWorld(Light1);
	scene1->addObjectToWorld(Light2);
	scene1->addObjectToWorld(Light3);

	_NL::Object::LightObject* Light4 = scene1->Instantiate<_NL::Object::LightObject>(Light3);
	Light4->LightProperties.lightPosition = glm::vec3(75, 5, -75);
	Light4->LightProperties.lightColor = glm::vec3(1000, 100, 100);

	_NL::Object::LightObject* Light5 = scene1->Instantiate<_NL::Object::LightObject>(Light3);
	Light5->LightProperties.lightPosition = glm::vec3(75, 5, 0);
	Light5->LightProperties.lightColor = glm::vec3(100, 1000, 100);





	//===========================================================================================
	//PARTICLE SYSTEMS - NOT WORKING...
	//===========================================================================================


	//_NL::Object::ParticleObj* flameParticle = new _NL::Object::ParticleObj();
	//
	//_NL::Element::TextureInstance* FlameAlbedo = new _NL::Element::TextureInstance("MyTexs/fire.png", false);
	//
	//_NL::Element::MaterialInstance* flameMat = new _NL::Element::MaterialInstance();
	//flameMat->LoadTexture(_NL::Element::AlbedoMap, FlameAlbedo, 0);
	//
	//flameParticle->addComponent(new _NL::Component::MeshRenderer);
	//flameParticle->addComponent(new _NL::Component::Transform);
	//flameParticle->getComponent<_NL::Component::MeshRenderer>()->Mesh = quadmesh;
	//flameParticle->getComponent<_NL::Component::MeshRenderer>()->Shader = simpleshade;
	//flameParticle->getComponent<_NL::Component::MeshRenderer>()->Material = flameMat;
	//flameParticle->getComponent<_NL::Component::MeshRenderer>()->initGLObj();
	//flameParticle->lifeTime = 100;
	//
	//_NL::Object::ParticleSystem* PS1 = new _NL::Object::ParticleSystem();
	//_NL::Component::CppScript<ParticleScript>* Pbehaviour = new _NL::Component::CppScript<ParticleScript>();
	//Pbehaviour->getScript()->W = &winMan;
	//PS1->Particle = flameParticle;
	//PS1->ParticlesBehavior = Pbehaviour->getScript();
	//PS1->SpawnerTransform.SpawnerShape = _NL::Object::ParticleSystem::SPHERE;
	//PS1->SpawnerTransform.Position.y = 10;
	//PS1->SpawnerTransform.Scale *= 0.1f;
	//PS1->SpawnerTransform.SpawnerHeightY = 10;
	//PS1->SpawnerTransform.SpawnerWidthX = 10;
	//PS1->SpawnerTransform.SpawnerWidthZ = 10;
	//PS1->SpawnerTransform.SpawnerConeVertexRadius = 0.0f;
	//PS1->TimeScale = &winMan.GameTime;
	//PS1->SpawnPerFrame = 5;
	//scene1->addObjectToWorld(PS1);

	//===========================================================================================
	//RUN SCENES
	//===========================================================================================

	winMan.RunScene(scene1);
	return 0;
}


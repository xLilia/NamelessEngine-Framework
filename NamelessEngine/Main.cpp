#pragma once
#include "NL.hpp"
#include "TestScript.hpp"
#include "BOIDscript.hpp"
#include "CamController.hpp"
#include "cockpitController.hpp"
#include "ParticleScript.hpp"
#include "GausianBlurPostProcessingEffect.hpp"
#include "MotionBlurPPE.hpp"
#include "NightVisionPPE.hpp"


int main() {
	//check_gl_error();

	//===========================================================================================
	//ENGINE NLManager SCENE 
	//===========================================================================================
	
	_NL::Engine::NLManager winMan("DEMO SCENE ALPHA 0.6", 1024, 768, true, true, false, true, false);
	_NL::Engine::WorldSpace* hangarScene = new _NL::Engine::WorldSpace;
	_NL::Engine::WorldSpace* CaveScene = new _NL::Engine::WorldSpace;
	//===========================================================================================
	//AUDIO: DEPRECATED ...
	//===========================================================================================

	_NL::Engine::AudioSource* Audio = new _NL::Engine::AudioSource;
	Audio->LoadAudioFile("Mysounds/ShipEngine.ogg");
	Audio->Sound.setLoop(true);
	Audio->Sound.play();

	//===========================================================================================
	//SKYBOXES: STABLE !
	//===========================================================================================
	
	_NL::Object::SkyboxObj* sky1 = new _NL::Object::SkyboxObj();
	_NL::Object::SkyboxObj* sky2= new _NL::Object::SkyboxObj();
	_NL::Object::SkyboxObj* sky3 = new _NL::Object::SkyboxObj();
	_NL::Element::ShaderInstance* SkyboxDysplayShader = new _NL::Element::ShaderInstance("Myshaders/SkyboxDefaultVertshader.glsl", "Myshaders/SkyboxDefaultFragshader.glsl");
	_NL::Element::ShaderInstance* HDRImageShader = new _NL::Element::ShaderInstance("Myshaders/HDRImageDefaultVertshader.glsl", "Myshaders/HDRImageDefaultFragshader.glsl");
	_NL::Element::ShaderInstance* EnvironmentShader = new _NL::Element::ShaderInstance("Myshaders/EnvironmentDefaultVertshader.glsl", "Myshaders/EnvironmentDefaultFragshader.glsl");
	_NL::Element::ShaderInstance* PreFilterShader = new _NL::Element::ShaderInstance("Myshaders/PreFilterDefaultVertshader.glsl", "Myshaders/PreFilterDefaultFragshader.glsl");
	_NL::Element::ShaderInstance* BRDFshader = new _NL::Element::ShaderInstance("Myshaders/BRDFDefaultVertshader.glsl", "Myshaders/BRDFDefaultFragshader.glsl");
	
	sky1->SkyboxDysplayShader = SkyboxDysplayShader;
	sky1->HDRImageShader = HDRImageShader;
	sky1->EnvironmentShader = EnvironmentShader;
	sky1->PreFilterShader = PreFilterShader;
	sky1->BRDFShader = BRDFshader;

	sky2->SkyboxDysplayShader = SkyboxDysplayShader;
	sky2->HDRImageShader = HDRImageShader;
	sky2->EnvironmentShader = EnvironmentShader;
	sky2->PreFilterShader = PreFilterShader;
	sky2->BRDFShader = BRDFshader;

	sky3->SkyboxDysplayShader = SkyboxDysplayShader;
	sky3->HDRImageShader = HDRImageShader;
	sky3->EnvironmentShader = EnvironmentShader;
	sky3->PreFilterShader = PreFilterShader;
	sky3->BRDFShader = BRDFshader;

	sky1->createEnvironment("Myhdri/hdri1.jpg",1024);
	sky1->createSkybox("Myhdri/hdri1.jpg",1024*2);

	sky2->createEnvironment("Myhdri/hdri2.jpg", 1024);
	sky2->createSkybox("Myhdri/hdri2.jpg", 1024 * 2);

	sky3->createEnvironment("Myhdri/hdri3.jpg", 1024);
	sky3->createSkybox("Myhdri/hdri3.jpg", 1024 * 2);

	hangarScene->Skybox = sky1;
	
	//===========================================================================================
	//MULTIPLE CAMERAS / VIEWPORTS: STABLE !
	//===========================================================================================
	
	_NL::Object::CameraObj* MyCam = new _NL::Object::CameraObj("MyCam", winMan.window->getSize().x, winMan.window->getSize().y, 0, 0, 90, 0.1, 1500, 1.0, 10, GL_LINEAR);
	_NL::Object::CameraObj* MyCam2 = new _NL::Object::CameraObj("MyCam2", winMan.window->getSize().x / 4, winMan.window->getSize().y / 4, winMan.window->getSize().x / 4 + winMan.window->getSize().x / 8, 0, 90, 0.1, 1500, 1.0, 10, GL_LINEAR);
		
	_NL::Element::ShaderInstance* screenshader = new _NL::Element::ShaderInstance("Myshaders/DdeferedScreenQuadvertexshader.glsl", "Myshaders/DdeferedScreenQuadfragmentshader.glsl");
	_NL::Element::ShaderInstance* GaussianBlur = new _NL::Element::ShaderInstance("Myshaders/GaussianBlurVshader.glsl", "Myshaders/GaussianBlurFshader.glsl");
	_NL::Element::ShaderInstance* NightVision = new _NL::Element::ShaderInstance("Myshaders/NightVisionPPEVertex.glsl", "Myshaders/NightVisionPPEFragment.glsl");
	_NL::Element::ShaderInstance* MotionBlur = new _NL::Element::ShaderInstance("Myshaders/MotionBlurPPEVertex.glsl", "Myshaders/MotionBlurPPEFragment.glsl");
	
	//GausianBlurPostProcessingEffect* GB = new GausianBlurPostProcessingEffect();
	//GB->PingPongIterations = 1;
	//GB->PingPongShader = GaussianBlur;
	//GB->TargetCam = MyCam;

	//MotionBlurPPE* MB = new MotionBlurPPE();
	//MB->MotionBlurShader = MotionBlur;
	//MB->TargetCam = MyCam;

	NightVisionPPE* NV = new NightVisionPPE();
	NV->NightVisionShader = NightVision;
	NV->TargetCam = MyCam;

	MyCam->addComponent(new _NL::Component::CppScript<CamController>);
	MyCam->getComponent<_NL::Component::CppScript<CamController>>()->getScript()->W = &winMan;
	MyCam->FinalPassShader = screenshader;
	MyCam->GenerateFrameBuffers();
	MyCam->PostProcessingStack.push_back(NV);
	hangarScene->addObjectToWorld(MyCam);
	CaveScene->addObjectToWorld(MyCam);

	MyCam2->FinalPassShader = screenshader;
	MyCam2->GenerateFrameBuffers();
	hangarScene->addObjectToWorld(MyCam2);
	CaveScene->addObjectToWorld(MyCam2);


	//===========================================================================================
	//2D CANVAS: 2D IMAGE RENDER - STABLE ! - TEXT RENDER - UNSTABLE... 
	//===========================================================================================
	
	_NL::Element::ShaderInstance* UITexShder = new _NL::Element::ShaderInstance("MyShaders/UITexVertshader.glsl", "MyShaders/UITexFragshader.glsl");
	_NL::UI::UICanvas* Canvas1 = new _NL::UI::UICanvas(winMan.window);
	Canvas1->ImageRenderShader = UITexShder;
	
	_NL::Element::TextureInstance* uiship0 = new _NL::Element::TextureInstance("myTexs/UI_ship1.png", 1);
	_NL::UI::UIImage* ui1 = new _NL::UI::UIImage(uiship0);
	ui1->scale.x *= 10;
	ui1->scale.y *= 10;
	ui1->AnchorPosition = glm::vec2(winMan.window->getSize().x,0);
	ui1->PositionRelativeToAnchor = glm::vec2(-ui1->widthHeight.x*ui1->scale.x, 0);
	ui1->Layer = 1;

	_NL::Element::TextureInstance* ui1rot = new _NL::Element::TextureInstance("myTexs/UI_ship1_rotator2.png", 1);
	_NL::UI::UIImage* ui1_rot1 = new _NL::UI::UIImage(ui1rot);
	ui1_rot1->scale = ui1->scale;
	ui1_rot1->AnchorPosition = ui1->AnchorPosition;
	ui1_rot1->PositionRelativeToAnchor = ui1->PositionRelativeToAnchor;
	ui1_rot1->Layer = 3;

	_NL::Element::TextureInstance* uirotmast = new _NL::Element::TextureInstance("myTexs/UI_ship1_rotMaster.png", 1);
	_NL::UI::UIImage* ui1_Mrot1 = new _NL::UI::UIImage(uirotmast);
	ui1_Mrot1->scale = ui1->scale;
	ui1_Mrot1->AnchorPosition = ui1->AnchorPosition;
	ui1_Mrot1->PositionRelativeToAnchor = ui1->PositionRelativeToAnchor;
	ui1_Mrot1->Layer = 2;

	_NL::Element::TextureInstance* ui1Fthrust = new _NL::Element::TextureInstance("myTexs/UI_ship1_FThrust.png", 1);
	_NL::UI::UIImage* ui1_Fthrust = new _NL::UI::UIImage(ui1Fthrust);
	ui1_Fthrust->scale = ui1->scale;
	ui1_Fthrust->AnchorPosition = ui1->AnchorPosition;
	ui1_Fthrust->PositionRelativeToAnchor = ui1->PositionRelativeToAnchor;
	ui1_Fthrust->Layer = 3;

	_NL::Element::TextureInstance* ui1Hthrust = new _NL::Element::TextureInstance("myTexs/UI_ship1_HThrust.png", 1);
	_NL::UI::UIImage* ui1_Hthrust = new _NL::UI::UIImage(ui1Hthrust);
	ui1_Hthrust->scale = ui1->scale;
	ui1_Hthrust->AnchorPosition = ui1->AnchorPosition;
	ui1_Hthrust->PositionRelativeToAnchor = ui1->PositionRelativeToAnchor;
	ui1_Hthrust->Layer = 3;

	_NL::Element::TextureInstance* ui1Vthrust = new _NL::Element::TextureInstance("myTexs/UI_ship1_VThrust.png", 1);
	_NL::UI::UIImage* ui1_Vthrust = new _NL::UI::UIImage(ui1Vthrust);
	ui1_Vthrust->scale = ui1->scale;
	ui1_Vthrust->AnchorPosition = ui1->AnchorPosition;
	ui1_Vthrust->PositionRelativeToAnchor = ui1->PositionRelativeToAnchor;
	ui1_Vthrust->Layer = 3;
	
	_NL::UI::UIText* ut1 = new _NL::UI::UIText("Myfonts/Consolas.ttf", "Hello World ! \(^_^)/ 12345");
	ut1->AnchorPosition = glm::vec2(100, 100);
	ut1->Layer = 4;

	Canvas1->addUIElement(ut1);
	Canvas1->addUIElement(ui1);
	Canvas1->addUIElement(ui1_rot1);
	Canvas1->addUIElement(ui1_Mrot1);
	Canvas1->addUIElement(ui1_Fthrust);
	Canvas1->addUIElement(ui1_Hthrust);
	Canvas1->addUIElement(ui1_Vthrust);
	
	hangarScene->addObjectToWorld(Canvas1);
	CaveScene->addObjectToWorld(Canvas1);

	//===========================================================================================
	//MATERIAL SHADERS - STABLE !
	//===========================================================================================
	_NL::Element::ShaderInstance* defaultshader = new _NL::Element::ShaderInstance("Myshaders/Ddeferedvertexshader.glsl", "Myshaders/Ddeferedfragmentshader.glsl");
	_NL::Element::ShaderInstance* MetalShader = new _NL::Element::ShaderInstance("Myshaders/Ddeferedvertexshader.glsl", "Myshaders/MetalMaterial.glsl");
	_NL::Element::ShaderInstance* RockShader = new _NL::Element::ShaderInstance("Myshaders/Ddeferedvertexshader.glsl", "Myshaders/RockMaterial.glsl");
	_NL::Element::ShaderInstance* PerfectMaterial1 = new _NL::Element::ShaderInstance("Myshaders/Ddeferedvertexshader.glsl", "Myshaders/perfectMaterial1.glsl");


	_NL::Element::MaterialInstance* RockMaterial = new _NL::Element::MaterialInstance();
	_NL::Element::TextureInstance* RockA = new _NL::Element::TextureInstance("MyTexs/Rock02/Rock02_A.jpg", 0);
	_NL::Element::TextureInstance* RockR = new  _NL::Element::TextureInstance("MyTexs/Rock02/Rock02_R.jpg", 0);
	_NL::Element::TextureInstance* RockM = new _NL::Element::TextureInstance("MyTexs/Rock02/Rock02_A.jpg", 0);
	_NL::Element::TextureInstance* RockN = new _NL::Element::TextureInstance("MyTexs/Rock02/Rock02_N.jpg", 0);
	_NL::Element::TextureInstance* RockAO = new _NL::Element::TextureInstance("MyTexs/Rock02/Rock02_AO.jpg", 0);
	RockMaterial->LoadTexture(_NL::Element::TEXTURE_TYPE::AlbedoMap, RockA, 0);
	RockMaterial->LoadTexture(_NL::Element::TEXTURE_TYPE::RoughnessMap, RockR, 0);
	RockMaterial->LoadTexture(_NL::Element::TEXTURE_TYPE::MetalnessMap, RockM, 0);
	RockMaterial->LoadTexture(_NL::Element::TEXTURE_TYPE::NormalMap, RockN, 0);
	RockMaterial->LoadTexture(_NL::Element::TEXTURE_TYPE::AmbientOcclusionMap, RockAO, 0);


	_NL::Element::MaterialInstance* MetalMaterial = new _NL::Element::MaterialInstance();
	_NL::Element::TextureInstance* MetA = new _NL::Element::TextureInstance("MyTexs/MetalPlates02/MetalPlates02_A.jpg", 0);
	_NL::Element::TextureInstance* MetR = new  _NL::Element::TextureInstance("MyTexs/MetalPlates02/MetalPlates02_R.jpg", 0);
	_NL::Element::TextureInstance* MetM = new _NL::Element::TextureInstance("MyTexs/MetalPlates02/MetalPlates02_M.jpg", 0);
	_NL::Element::TextureInstance* MetN = new _NL::Element::TextureInstance("MyTexs/MetalPlates02/MetalPlates02_N.jpg", 0);
	MetalMaterial->LoadTexture(_NL::Element::TEXTURE_TYPE::AlbedoMap, MetA, 0);
	MetalMaterial->LoadTexture(_NL::Element::TEXTURE_TYPE::RoughnessMap, MetR, 0);
	MetalMaterial->LoadTexture(_NL::Element::TEXTURE_TYPE::MetalnessMap, MetM, 0);
	MetalMaterial->LoadTexture(_NL::Element::TEXTURE_TYPE::NormalMap, MetN, 0);


	_NL::Element::MaterialInstance* MetalMaterial2 = new _NL::Element::MaterialInstance();
	_NL::Element::TextureInstance* Met2A = new _NL::Element::TextureInstance("MyTexs/PaintedMetal04/PaintedMetal04_A.jpg", 0);
	_NL::Element::TextureInstance* Met2R = new  _NL::Element::TextureInstance("MyTexs/PaintedMetal04/PaintedMetal04_R.jpg", 0);
	_NL::Element::TextureInstance* Met2M = new _NL::Element::TextureInstance("MyTexs/PaintedMetal04/PaintedMetal04_M.jpg", 0);
	_NL::Element::TextureInstance* Met2N = new _NL::Element::TextureInstance("MyTexs/PaintedMetal04/PaintedMetal04_N.jpg", 0);
	MetalMaterial2->LoadTexture(_NL::Element::TEXTURE_TYPE::AlbedoMap, Met2A, 0);
	MetalMaterial2->LoadTexture(_NL::Element::TEXTURE_TYPE::RoughnessMap, Met2R, 0);
	MetalMaterial2->LoadTexture(_NL::Element::TEXTURE_TYPE::MetalnessMap, Met2M, 0);
	MetalMaterial2->LoadTexture(_NL::Element::TEXTURE_TYPE::NormalMap, Met2N, 0);

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

	//(Player )===========================================================================================

	_NL::Object::GameObject* MechaChair = new _NL::Object::GameObject("MechaChair");
	_NL::Element::MeshInstance* MechaChairMesh = new _NL::Element::MeshInstance("MyModels/Mecha_Chair.obj");
	MechaChair->addComponent(new _NL::Component::Transform);
	MechaChair->addComponent(new _NL::Component::MeshRenderer);
	MechaChair->getComponent<_NL::Component::MeshRenderer>()->Mesh = MechaChairMesh;
	MechaChair->getComponent<_NL::Component::MeshRenderer>()->Shader = defaultshader;
	MechaChair->getComponent<_NL::Component::MeshRenderer>()->Material = PBRGunMAT;
	hangarScene->addObjectToWorld(MechaChair);
	CaveScene->addObjectToWorld(MechaChair);

	_NL::Object::GameObject* Mecha = new _NL::Object::GameObject("Mecha1");
	_NL::Element::MeshInstance* Mechamesh = new _NL::Element::MeshInstance("MyModels/Mecha1-3.obj");
	Mecha->addComponent(new _NL::Component::Transform);
	Mecha->addComponent(new _NL::Component::MeshRenderer);
	Mecha->addComponent(new _NL::Component::CppScript<CockpitController>);
	Mecha->getComponent<_NL::Component::MeshRenderer>()->Mesh = Mechamesh;
	Mecha->getComponent<_NL::Component::MeshRenderer>()->Shader = defaultshader;
	Mecha->getComponent<_NL::Component::MeshRenderer>()->Material = PBRGunMAT;
	Mecha->getComponent<_NL::Component::CppScript<CockpitController>>()->getScript()->M = &winMan;
	Mecha->getComponent<_NL::Component::CppScript<CockpitController>>()->getScript()->PLcam = MyCam;
	Mecha->getComponent<_NL::Component::CppScript<CockpitController>>()->getScript()->PLBackcam = MyCam2;
	Mecha->getComponent<_NL::Component::CppScript<CockpitController>>()->getScript()->Chair = MechaChair;

	_NL::Object::LightObject* Light1 = new _NL::Object::LightObject("Light1");
	hangarScene->addObjectToWorld(Light1);
	CaveScene->addObjectToWorld(Light1);

	_NL::Object::LightObject* Light2 = new _NL::Object::LightObject("Light2");
	Light2->addComponent(new _NL::Component::Transform);
	Light2->getComponent<_NL::Component::Transform>()->transform.position.y = 0;
	Light2->getComponent<_NL::Component::Transform>()->transform.position.z = 0;
	Light2->getComponent<_NL::Component::Transform>()->transform.position.x = 2;
	Light2->Parent = Mecha;
	hangarScene->addObjectToWorld(Light2);
	CaveScene->addObjectToWorld(Light2);

	_NL::Object::LightObject* Light3 = new _NL::Object::LightObject("Light3");
	Light3->addComponent(new _NL::Component::Transform);
	Light3->getComponent<_NL::Component::Transform>()->transform.position.y = 0;
	Light3->getComponent<_NL::Component::Transform>()->transform.position.z = 0;
	Light3->getComponent<_NL::Component::Transform>()->transform.position.x = -2;
	Light3->Parent = Mecha;
	hangarScene->addObjectToWorld(Light3);
	CaveScene->addObjectToWorld(Light3);

	_NL::Object::LightObject* Light4 = new _NL::Object::LightObject("Light4");
	hangarScene->addObjectToWorld(Light4);
	CaveScene->addObjectToWorld(Light4);

	_NL::Object::LightObject* Light5 = new _NL::Object::LightObject("Light5");
	hangarScene->addObjectToWorld(Light5);
	CaveScene->addObjectToWorld(Light5);

	Mecha->getComponent<_NL::Component::CppScript<CockpitController>>()->getScript()->CockpitLight1 = Light1;
	Mecha->getComponent<_NL::Component::CppScript<CockpitController>>()->getScript()->LeftLight = Light2;
	Mecha->getComponent<_NL::Component::CppScript<CockpitController>>()->getScript()->RightLight = Light3;
	Mecha->getComponent<_NL::Component::CppScript<CockpitController>>()->getScript()->CenterLight = Light4;
	Mecha->getComponent<_NL::Component::CppScript<CockpitController>>()->getScript()->BottomLight1 = Light5;
	Mecha->getComponent<_NL::Component::CppScript<CockpitController>>()->getScript()->Hangarlevel = hangarScene;
	Mecha->getComponent<_NL::Component::CppScript<CockpitController>>()->getScript()->CaveLevel = CaveScene;
	Mecha->getComponent<_NL::Component::CppScript<CockpitController>>()->getScript()->ui1_rotator = ui1_rot1;
	Mecha->getComponent<_NL::Component::CppScript<CockpitController>>()->getScript()->ui1_rotatorMast = ui1_Mrot1;
	Mecha->getComponent<_NL::Component::CppScript<CockpitController>>()->getScript()->ui1_Fthrust = ui1_Fthrust;
	Mecha->getComponent<_NL::Component::CppScript<CockpitController>>()->getScript()->ui1_Hthrust = ui1_Hthrust;
	Mecha->getComponent<_NL::Component::CppScript<CockpitController>>()->getScript()->ui1_Vthrust = ui1_Vthrust;
	Mecha->getComponent<_NL::Component::CppScript<CockpitController>>()->getScript()->sky1 = sky1;
	Mecha->getComponent<_NL::Component::CppScript<CockpitController>>()->getScript()->sky2 = sky2;
	Mecha->getComponent<_NL::Component::CppScript<CockpitController>>()->getScript()->sky3 = sky3;
	hangarScene->addObjectToWorld(Mecha);
	CaveScene->addObjectToWorld(Mecha);

	//(LVL1 )===========================================================================================
	_NL::Element::MeshInstance* hangarmesh = new _NL::Element::MeshInstance("MyModels/LVL1/Lvl_1_hangar.obj");
	_NL::Object::GameObject* hangar = new _NL::Object::GameObject("hangar");
	hangar->addComponent(new _NL::Component::Transform());
	hangar->addComponent(new _NL::Component::MeshRenderer());
	hangar->getComponent<_NL::Component::MeshRenderer>()->Mesh = hangarmesh;
	hangar->getComponent<_NL::Component::MeshRenderer>()->Shader = MetalShader;
	hangar->getComponent<_NL::Component::MeshRenderer>()->Material = MetalMaterial;
	hangar->getComponent<_NL::Component::MeshRenderer>()->GL_CullFace = true;

	_NL::Element::MeshInstance* hangarDoorsmesh = new _NL::Element::MeshInstance("MyModels/LVL1/Lvl_1_hangarDoors.obj");
	_NL::Object::GameObject* hangarDoors = new _NL::Object::GameObject("hangarDoors");
	hangarDoors->addComponent(new _NL::Component::Transform());
	hangarDoors->addComponent(new _NL::Component::MeshRenderer());
	hangarDoors->getComponent<_NL::Component::MeshRenderer>()->Mesh = hangarDoorsmesh;
	hangarDoors->getComponent<_NL::Component::MeshRenderer>()->Shader = MetalShader;
	hangarDoors->getComponent<_NL::Component::MeshRenderer>()->Material = MetalMaterial2;
	hangarDoors->getComponent<_NL::Component::MeshRenderer>()->GL_CullFace = true;

	_NL::Element::MeshInstance* hangarTubeMesh = new _NL::Element::MeshInstance("MyModels/LVL1/Lvl_1_hangarTube.obj");
	_NL::Object::GameObject* hangarTube = new _NL::Object::GameObject("hangarTube");
	hangarTube->addComponent(new _NL::Component::Transform());
	hangarTube->addComponent(new _NL::Component::MeshRenderer());
	hangarTube->getComponent<_NL::Component::MeshRenderer>()->Mesh = hangarTubeMesh;
	hangarTube->getComponent<_NL::Component::MeshRenderer>()->Shader = MetalShader;
	hangarTube->getComponent<_NL::Component::MeshRenderer>()->Material = MetalMaterial;
	hangarTube->getComponent<_NL::Component::MeshRenderer>()->GL_CullFace = true;

	_NL::Element::MeshInstance* hangarSymbolmesh = new _NL::Element::MeshInstance("MyModels/LVL1/Lvl_1_hangarSymbol.obj");
	_NL::Object::GameObject* hangarSymbol = new _NL::Object::GameObject("hangarSymbol");
	hangarSymbol->addComponent(new _NL::Component::Transform());
	hangarSymbol->addComponent(new _NL::Component::MeshRenderer());
	hangarSymbol->getComponent<_NL::Component::MeshRenderer>()->Mesh = hangarSymbolmesh;
	hangarSymbol->getComponent<_NL::Component::MeshRenderer>()->Shader = MetalShader;
	hangarSymbol->getComponent<_NL::Component::MeshRenderer>()->Material = MetalMaterial;
	hangarSymbol->getComponent<_NL::Component::MeshRenderer>()->GL_CullFace = true;

	_NL::Element::MeshInstance* hangarBall0Mesh = new _NL::Element::MeshInstance("MyModels/LVL1/Lvl_1_hangarBall_0.obj");
	_NL::Object::GameObject* hangarBall0 = new _NL::Object::GameObject("hangarBall0");
	hangarBall0->addComponent(new _NL::Component::Transform());
	hangarBall0->addComponent(new _NL::Component::MeshRenderer());
	hangarBall0->getComponent<_NL::Component::MeshRenderer>()->Mesh = hangarBall0Mesh;
	hangarBall0->getComponent<_NL::Component::MeshRenderer>()->Shader = MetalShader;
	hangarBall0->getComponent<_NL::Component::MeshRenderer>()->Material = MetalMaterial;
	hangarBall0->getComponent<_NL::Component::MeshRenderer>()->GL_CullFace = true;

	_NL::Element::MeshInstance* hangarBall1Mesh = new _NL::Element::MeshInstance("MyModels/LVL1/Lvl_1_hangarBall_1.obj");
	_NL::Object::GameObject* hangarBall1 = new _NL::Object::GameObject("hangarBall1");
	hangarBall1->addComponent(new _NL::Component::Transform());
	hangarBall1->addComponent(new _NL::Component::MeshRenderer());
	hangarBall1->getComponent<_NL::Component::MeshRenderer>()->Mesh = hangarBall1Mesh;
	hangarBall1->getComponent<_NL::Component::MeshRenderer>()->Shader = MetalShader;
	hangarBall1->getComponent<_NL::Component::MeshRenderer>()->Material = MetalMaterial2;
	hangarBall1->getComponent<_NL::Component::MeshRenderer>()->GL_CullFace = true;

	_NL::Element::MeshInstance* hangarBall2Mesh = new _NL::Element::MeshInstance("MyModels/LVL1/Lvl_1_hangarBall_2.obj");
	_NL::Object::GameObject* hangarBall2 = new _NL::Object::GameObject("hangarBall2");
	hangarBall2->addComponent(new _NL::Component::Transform());
	hangarBall2->addComponent(new _NL::Component::MeshRenderer());
	hangarBall2->getComponent<_NL::Component::MeshRenderer>()->Mesh = hangarBall2Mesh;
	hangarBall2->getComponent<_NL::Component::MeshRenderer>()->Shader = RockShader;
	hangarBall2->getComponent<_NL::Component::MeshRenderer>()->Material = RockMaterial;
	hangarBall2->getComponent<_NL::Component::MeshRenderer>()->GL_CullFace = true;

	_NL::Element::MeshInstance* PBRGunMesh = new _NL::Element::MeshInstance("MyModels/LVL1/Lvl_1_hangarGUN.obj");
	_NL::Object::GameObject* PBRGun = new _NL::Object::GameObject("PBRGun");
	PBRGun->addComponent(new _NL::Component::Transform());
	PBRGun->addComponent(new _NL::Component::MeshRenderer());
	PBRGun->getComponent<_NL::Component::MeshRenderer>()->Mesh = PBRGunMesh;
	PBRGun->getComponent<_NL::Component::MeshRenderer>()->Shader = defaultshader;
	PBRGun->getComponent<_NL::Component::MeshRenderer>()->Material = PBRGunMAT;
	PBRGun->getComponent<_NL::Component::MeshRenderer>()->GL_CullFace = true;

	_NL::Element::MeshInstance* Cave_0_mesh = new _NL::Element::MeshInstance("MyModels/LVL1/Lvl_1_Cave_0.obj");
	_NL::Object::GameObject* Cave_0 = new _NL::Object::GameObject("Cave_0");
	Cave_0->addComponent(new _NL::Component::Transform());
	Cave_0->addComponent(new _NL::Component::MeshRenderer());
	Cave_0->getComponent<_NL::Component::MeshRenderer>()->Mesh = Cave_0_mesh;
	Cave_0->getComponent<_NL::Component::MeshRenderer>()->Shader = RockShader;
	Cave_0->getComponent<_NL::Component::MeshRenderer>()->Material = RockMaterial;
	Cave_0->getComponent<_NL::Component::MeshRenderer>()->GL_CullFace = true;

	_NL::Element::MeshInstance* Cave_Cube_mesh = new _NL::Element::MeshInstance("MyModels/LVL1/Lvl_1_Cave_Cube.obj");
	_NL::Object::GameObject* Cave_Cube = new _NL::Object::GameObject("Cave_Cube");
	Cave_Cube->addComponent(new _NL::Component::Transform());
	Cave_Cube->addComponent(new _NL::Component::MeshRenderer());
	Cave_Cube->getComponent<_NL::Component::MeshRenderer>()->Mesh = Cave_Cube_mesh;
	Cave_Cube->getComponent<_NL::Component::MeshRenderer>()->Shader = PerfectMaterial1;
	Cave_Cube->getComponent<_NL::Component::MeshRenderer>()->GL_CullFace = true;

	hangarScene->addObjectToWorld(hangar);
	hangarScene->addObjectToWorld(hangarSymbol);
	hangarScene->addObjectToWorld(hangarDoors);
	hangarScene->addObjectToWorld(hangarTube);
	hangarScene->addObjectToWorld(hangarBall0);
	hangarScene->addObjectToWorld(hangarBall1);
	hangarScene->addObjectToWorld(hangarBall2);
	hangarScene->addObjectToWorld(PBRGun);
	hangarScene->addObjectToWorld(Cave_Cube);
	CaveScene->addObjectToWorld(hangarDoors);
	CaveScene->addObjectToWorld(hangarTube);
	CaveScene->addObjectToWorld(Cave_0);
	CaveScene->addObjectToWorld(Cave_Cube);

	//(3)===========================================================================================

	//_NL::Object::GameObject* BOIDsBRAIN = new _NL::Object::GameObject();
	//_NL::Component::CppScript<BOIDscript>* BOIDsBRAINscript = BOIDsBRAIN->addComponent(new _NL::Component::CppScript<BOIDscript>);
	//BOIDsBRAINscript->getScript()->W = &winMan;

	_NL::Element::MeshInstance* boidmesh = new _NL::Element::MeshInstance("MyModels/BOID3.obj");

	for (int n = 0; n < 50; n++) {
		BOIDObject* BOID1 = new BOIDObject();
		BOID1->name = "Boid_Group1";
		BOID1->Mesh = boidmesh;
		BOID1->Shader = PerfectMaterial1;
		BOID1->Material = RockMaterial;
		BOID1->Mr->GL_CullFace = false;
		BOID1->UpdateMeshRenderer();
		BOID1->T->transform.position.z = n % 3;
		BOID1->T->transform.position.x = n % 5;
		BOID1->T->transform.position.y = n % 4;
		BOID1->T->transform.scale *= .5;
		_NL::Component::CppScript<BOIDscript>* BOIDsBRAINscript = BOID1->addComponent(new _NL::Component::CppScript<BOIDscript>);
		BOIDsBRAINscript->getScript()->W = &winMan;
		hangarScene->addObjectToWorld(BOID1);
	}
	
	for (int n = 0; n < 50; n++) {
		BOIDObject* BOID2 = new BOIDObject();
		BOID2->name = "Boid_Group2";
		BOID2->Mesh = boidmesh;
		BOID2->Shader = PerfectMaterial1;
		BOID2->Material = RockMaterial;
		BOID2->Mr->GL_CullFace = false;
		BOID2->UpdateMeshRenderer();
		BOID2->T->transform.position.z = n % 5;
		BOID2->T->transform.position.x = n % 3;
		BOID2->T->transform.position.y = n % 4;
		BOID2->T->transform.scale *= .5;
		_NL::Component::CppScript<BOIDscript>* BOIDsBRAINscript = BOID2->addComponent(new _NL::Component::CppScript<BOIDscript>);
		BOIDsBRAINscript->getScript()->W = &winMan;
		hangarScene->addObjectToWorld(BOID2);
	}

	for (int n = 0; n < 50; n++) {
		BOIDObject* BOID3 = new BOIDObject();
		BOID3->name = "Boid_Group3";
		BOID3->Mesh = boidmesh;
		BOID3->Shader = PerfectMaterial1;
		BOID3->Material = RockMaterial;
		BOID3->Mr->GL_CullFace = false;
		BOID3->UpdateMeshRenderer();
		BOID3->T->transform.position.z = n % 4;
		BOID3->T->transform.position.x = n % 6;
		BOID3->T->transform.position.y = n % 2;
		BOID3->T->transform.scale *= .5;
		_NL::Component::CppScript<BOIDscript>* BOIDsBRAINscript = BOID3->addComponent(new _NL::Component::CppScript<BOIDscript>);
		BOIDsBRAINscript->getScript()->W = &winMan;
		hangarScene->addObjectToWorld(BOID3);
	}

	

	//for (int n = 0; n < 10; n++) {
	//	BOIDObject* b = scene1->Instantiate<BOIDObject>(BOID3);
	//	b->UpdateTransform();
	//	 
	//	//_NL::Object::GameObject* b = winMan.SafeInstantiateObj<_NL::Object::GameObject>(BOID);
	//	//_NL::Component::Transform* T = b->getComponent<_NL::Component::Transform>();
	//	//T->bactive = false;
	//	//T->transform.position.x = n;
	//}

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
	winMan.CurrentScene = hangarScene;
	winMan.RunScene(hangarScene);
	return 0;
}


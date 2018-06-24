#pragma once
#include "NL.hpp"
#include "MotionBlurPPE.hpp"
class CockpitController : public _NL::Core::Script<_NL::Object::GameObject>
{
public:

	_NL::Object::CameraObj* PLcam;
	_NL::Object::CameraObj* PLBackcam;

	_NL::Object::SkyboxObj* sky1;
	_NL::Object::SkyboxObj* sky2;
	_NL::Object::SkyboxObj* sky3;

	_NL::Object::GameObject* Chair;
	_NL::Component::Transform* T;
	_NL::Component::Transform* Tchair;
	_NL::Object::LightObject * CockpitLight1;
	_NL::Object::LightObject * LeftLight;
	_NL::Object::LightObject * RightLight;
	_NL::Object::LightObject * CenterLight;
	_NL::Object::LightObject * BottomLight1;
	_NL::UI::UIImage* ui1_rotator;
	_NL::UI::UIImage* ui1_rotatorMast;
	_NL::UI::UIImage* ui1_Fthrust;
	_NL::UI::UIImage* ui1_Hthrust;
	_NL::UI::UIImage* ui1_Vthrust;

	_NL::Engine::WorldSpace*Hangarlevel;
	_NL::Engine::WorldSpace*CaveLevel;

	_NL::Engine::NLManager* M;
	
	GLuint ActiveCam = 1;
	GLuint ActiveSky = 1;
	bool NightVisionON = false;
	bool FreecamON;
	bool FlightAssistON = true;
	GLuint FreecamMode = 0;
	float dt; //deltaTime
	glm::vec3 CockpitCamPos = glm::vec3(0, 1.3, -0.5);
	glm::vec3 CockpitCamAbove = glm::vec3(0, 3, -0.5);
	glm::vec3 CockpitCamBelow = glm::vec3(0, -0.5, -0.5);
	glm::vec3 NightVisionCamPos = glm::vec3(0, 3, 0);
	glm::vec3 velocity;
	float YawVelocity;
	float camZoom = 0;

	void movement();
	void cameraSetup();
	void UpdateUI();
	void sceneManager();

	void Start() override;
	void Update() override;
};

void CockpitController::Start() {
	T = _this->getComponent<_NL::Component::Transform>();
	Tchair = Chair->getComponent<_NL::Component::Transform>();
}
void CockpitController::sceneManager() {

	if (M->CurrentScene != CaveLevel){

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9)) {
			ActiveSky = 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8)) {
			ActiveSky = 2;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)) {
			ActiveSky = 3;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)) {
			ActiveSky = 0;
		}


		switch (ActiveSky)
		{
		case 1:
			M->CurrentScene->Skybox = sky1;
			break;
		case 2:
			M->CurrentScene->Skybox = sky2;
			break;
		case 3:
			M->CurrentScene->Skybox = sky3;
			break;
		default:
			M->CurrentScene->Skybox = nullptr;
			break;
		}

		if (PLcam->transformCam.Position.z < -127.0f) {
			M->EndCurrentScene(CaveLevel);
		}
	}

	if (M->CurrentScene != Hangarlevel && PLcam->transformCam.Position.z > -127.0f) {
		M->EndCurrentScene(Hangarlevel);
	}

}

void CockpitController::movement() {
	
	float s0 = 0.1 * dt;
	float s1 = 0.05 * dt;
	float s2 = 0.01 * dt;
	bool breaks = false;

	//------------------------------------------------------------
	//------------------------------------------------------------
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
		velocity += glm::vec3(0, s0, 0);
	}
	else if(FlightAssistON){
		velocity.y > 0.0f ? velocity.y -= s0 : 0;
	}
	//------------------------------------------------------------
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
		velocity += glm::vec3(0, -s0, 0);
	}
	else if (FlightAssistON) {
		velocity.y < 0.0f ? velocity.y += s0 : 0;
	}
	//------------------------------------------------------------
 	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		velocity += glm::vec3(0, 0, -s1);
	}
	else if (FlightAssistON) {
		velocity.z < 0.0f ? velocity.z += s1 : 0;
	}
	//------------------------------------------------------------
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		velocity += glm::vec3(0, 0, s1);
	}
	else if (FlightAssistON) {
		velocity.z > 0.0f ? velocity.z -= s1 : 0;
	}
	//------------------------------------------------------------
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		YawVelocity += s2;
	}
	else if(FlightAssistON) {
		YawVelocity > 0.0f ? YawVelocity -= s2 : 0;
	}
	//------------------------------------------------------------
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		YawVelocity -= s2;
	}
	else if (FlightAssistON) {
		YawVelocity < 0.0f ? YawVelocity += s2 : 0;
	}
	//------------------------------------------------------------
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		velocity += glm::vec3(-s0, 0, 0);
	}
	else if (FlightAssistON) {
		velocity.x < 0.0f ? velocity.x += s0 : 0;
	}
	//------------------------------------------------------------
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		velocity += glm::vec3(s0, 0, 0);
	}
	else if (FlightAssistON) {
		velocity.x > 0.0f ? velocity.x -= s0 : 0;
	}
	//------------------------------------------------------------
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		breaks = true;
	}

	//------------------------------------------------------------
	velocity = glm::clamp(velocity, -0.2f, 0.2f);
	YawVelocity = glm::clamp(YawVelocity, -0.01f, 0.01f);

	if (breaks) {
		
		velocity.x > 0.0f ? velocity.x -= s0 : 0;
		velocity.x < 0.0f ? velocity.x += s0 : 0;
		velocity.y > 0.0f ? velocity.y -= s0 : 0;
		velocity.y < 0.0f ? velocity.y += s0 : 0;
		velocity.z > 0.0f ? velocity.z -= s1 : 0;
		velocity.z < 0.0f ? velocity.z += s1 : 0;
		YawVelocity > 0.0f ? YawVelocity -= s2 : 0;
		YawVelocity < 0.0f ? YawVelocity += s2 : 0;
	}


	T->transform.QuaternionRotation *= T->EulerRotation(0, YawVelocity, 0);
	T->transform.position += T->transform.QuaternionRotation * velocity;

}


void CockpitController::cameraSetup() {

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		camZoom += 1.0f;
		camZoom = glm::clamp(camZoom, 0.0f, 85.0f);
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		camZoom -= 1.0f;
		camZoom = glm::clamp(camZoom, 0.0f, 85.0f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
		PLcam->getComponent<_NL::Component::CppScript<CamController>>()->getScript()->freecamON = false;
		FreecamON = false;
		ActiveCam = 1;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0)) {
		PLcam->getComponent<_NL::Component::CppScript<CamController>>()->getScript()->freecamON = true;
		FreecamON = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F3)) {
		ActiveCam = 2;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F3)) {
		ActiveCam = 2;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F4)) {
		ActiveCam = 3;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F5)) {
		NightVisionON = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F6)) {
		NightVisionON = true;
	}

	glm::vec3 CKPL1T = T->transform.position + CockpitCamPos;
	glm::vec3 LLT = LeftLight->getComponent<_NL::Component::Transform>()->getModelMatrixTranslation();
	glm::vec3 RLT = RightLight->getComponent<_NL::Component::Transform>()->getModelMatrixTranslation();
	glm::vec3 CamRot;

	if(!FreecamON)
	switch (ActiveCam)
	{
	case 1:
		//Cams
		PLcam->FOV = 90;
		PLBackcam->bactive = false;
		if (PLcam->RenderScaleRatio != 1.0f) {
			PLcam->TextureFiltering = GL_LINEAR;
			PLcam->RenderScaleRatio = 1.0f;
			PLcam->GenerateFrameBuffers();
		}
		PLcam->transformCam.Position = T->transform.position + T->getParentedVector(CockpitCamPos);
		//Objs
		Tchair->transform.position = PLcam->transformCam.Position - glm::vec3(0, 1.3, 0);
		CamRot = glm::vec3(PLcam->transformCam.LookAt.x, 0, PLcam->transformCam.LookAt.z) - T->transform.position;
		Tchair->transform.QuaternionRotation = Tchair->LookAt(-CamRot, 0, 0, 1);
		break;
	case 2:
		//Cams
		PLcam->FOV = 90 - camZoom;
		if (PLcam->RenderScaleRatio != 0.3f) {
			PLcam->TextureFiltering = GL_NEAREST;
			PLcam->RenderScaleRatio = 0.3f;
			PLcam->GenerateFrameBuffers();
		}
		PLcam->transformCam.Position = T->transform.position + T->getParentedVector(NightVisionCamPos);
		PLBackcam->bactive = true;
		PLBackcam->transformCam.Position = T->transform.position + T->getParentedVector(CockpitCamPos);
		PLBackcam->transformCam.LookAt = PLcam->transformCam.LookAt;
		//Objs
		Tchair->transform.position = PLBackcam->transformCam.Position - glm::vec3(0, 1.3, 0);
		CamRot = glm::vec3(PLBackcam->transformCam.LookAt.x, 0, PLBackcam->transformCam.LookAt.z) - T->transform.position;
		Tchair->transform.QuaternionRotation = Tchair->LookAt(-CamRot, 0, 0, 1);
		break;
	case 3:
		//Cams
		PLcam->FOV = 90 - camZoom;
		PLBackcam->bactive = false;
		if (PLcam->RenderScaleRatio != 0.3f) {
			PLcam->TextureFiltering = GL_NEAREST;
			PLcam->RenderScaleRatio = 0.3f;
			PLcam->GenerateFrameBuffers();
		}
		PLcam->transformCam.Position = T->transform.position + T->getParentedVector(CockpitCamBelow);
		PLBackcam->bactive = true;
		PLBackcam->transformCam.Position = T->transform.position + T->getParentedVector(CockpitCamPos);
		PLBackcam->transformCam.LookAt = PLcam->transformCam.LookAt;
		//Objs
		Tchair->transform.position = PLBackcam->transformCam.Position - glm::vec3(0, 1.3, 0);
		CamRot = glm::vec3(PLBackcam->transformCam.LookAt.x, 0, PLBackcam->transformCam.LookAt.z) - T->transform.position;
		Tchair->transform.QuaternionRotation = Tchair->LookAt(-CamRot, 0, 0, 1);
		break;
	default:
		break;
	}

	//Lights
	if (!FreecamON) {
		if (NightVisionON && ActiveCam > 1) {
			LeftLight->bactive = false;
			RightLight->bactive = false;
			CenterLight->bactive = false;
			CockpitLight1->LightProperties.setPointLightProperties(glm::vec3(25, 0, 0), CKPL1T);
			BottomLight1->bactive = true;
			BottomLight1->LightProperties.setSpotLightProperties(glm::vec3(50 + camZoom * 50, 0, 0), T->transform.position + T->getParentedVector(glm::vec3(0, 0, -3)), PLcam->transformCam.LookAt/*T->getParentedVector(glm::vec3(0, 0, -1))*/, 0, 45);
			PLcam->PostProcessingStack[0]->bactive = true;
		}
		else {
			CockpitLight1->LightProperties.setPointLightProperties(glm::vec3(0.3, 0.4, 0.5), CKPL1T);
			LeftLight->bactive = true;
			LeftLight->LightProperties.setSpotLightProperties(glm::vec3(1000), LLT, T->getParentedVector(glm::vec3(-.5, 0, -1)), 12, 20);
			RightLight->bactive = true;
			RightLight->LightProperties.setSpotLightProperties(glm::vec3(1000), RLT, T->getParentedVector(glm::vec3(.5, 0, -1)), 12, 20);
			CenterLight->bactive = true;
			CenterLight->LightProperties.setSpotLightProperties(glm::vec3(1000), T->transform.position + glm::vec3(0, -1, 0), PLcam->transformCam.LookAt, 12, 25);

			if (ActiveCam == 1) {
				CockpitLight1->bactive = true;
				BottomLight1->bactive = false;
			}

			if (ActiveCam == 2) {
				BottomLight1->bactive = true;
				BottomLight1->LightProperties.setPointLightProperties(glm::vec3(0.3, 0.4, 0.5), PLcam->transformCam.Position);
			}

			if (ActiveCam == 3) {
				BottomLight1->bactive = true;
				BottomLight1->LightProperties.setPointLightProperties(glm::vec3(0.3, 0.4, 0.5), PLcam->transformCam.Position);
			}
			PLcam->PostProcessingStack[0]->bactive = false;
		}
	}
	else {
		//FREECAM ON
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
			FreecamMode = 0;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
			FreecamMode = 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
			FreecamMode = 2;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
			FreecamMode = 3;
		}
		float r,g,b;
		switch (FreecamMode)
		{
		case 0:
			CockpitLight1->bactive = false;
			break;
		case 1:
			CockpitLight1->bactive = true;
			CockpitLight1->LightProperties.setPointLightProperties(glm::vec3(50, 50, 50), PLcam->transformCam.Position);
			break;
		case 2:
			CockpitLight1->bactive = true;
			r = abs(sin(0.3 * M->GameTime.Clock.getElapsedTime().asSeconds() + 0));
			g = abs(sin(0.3 * M->GameTime.Clock.getElapsedTime().asSeconds() + 2));
			b = abs(sin(0.3 * M->GameTime.Clock.getElapsedTime().asSeconds() + 4));
			CockpitLight1->LightProperties.setPointLightProperties(glm::vec3(r, g, b)*50.0f, PLcam->transformCam.Position);
			break;
		case 3:
			CockpitLight1->bactive = true;
			CockpitLight1->LightProperties.setSpotLightProperties(glm::vec3(500, 500, 500), PLcam->transformCam.Position,PLcam->transformCam.LookAt,0,40);
			break;
		default:
			break;
		}
	}

}

void CockpitController::UpdateUI() {
	//UI
	float maxUIMov = ((ui1_rotator->widthHeight.y*ui1_rotator->scale.y) / 2.0) - ui1_rotator->widthHeight.y * 2.5;
	ui1_Fthrust->PositionRelativeToAnchor.y = maxUIMov * (1.0 / (0.2 * (1.0 / velocity.z)));
	ui1_Hthrust->PositionRelativeToAnchor.x = -ui1_Hthrust->widthHeight.x*ui1_Hthrust->scale.x + (maxUIMov * (1.0 / (0.2 * (1.0 / velocity.x))));
	ui1_Vthrust->PositionRelativeToAnchor.y = maxUIMov * (1.0 / (0.2 * (1.0 / -velocity.y)));
	
	glm::vec3 lookat = glm::normalize(glm::vec3(PLcam->transformCam.LookAt.x, 0, PLcam->transformCam.LookAt.z));
	float camAngle = glm::degrees(glm::acos(glm::dot(lookat, glm::vec3(0, 0, -1.0f))));
	glm::vec3 C = glm::cross(lookat, glm::vec3(0, 0, -1.0f));
	float WindingDir = glm::dot(C, glm::vec3(0, -1.0f, 0));
	WindingDir > 0 ? camAngle : camAngle *= -1;

	ui1_rotator->RotationAngle += YawVelocity * 1000;
	ui1_rotatorMast->RotationAngle = camAngle;
}

void CockpitController::Update() {
	sceneManager();

	dt = M->GameTime.DeltaTime.asSeconds();
	
	if (!FreecamON) {
		movement();
		UpdateUI();
	}
	cameraSetup();
	
	//std::cout <<  << std::endl;
}

/*Controls:

F1- CockpitCamera
Number 0: FreeCamera

when on CockpitCamera
F3- Top camera
F4- BottomCamera
WS - Forward / Backward
AD - Turn Left / Turn Right
QE - Strife Left / Strife Right
LShift LControll - Up / Down
Spacebar : Breaks

when on Top or bottom camera:
F5 - Normal Vision
F6 - NightVision
when on Night Vision:
LeftRight Mouse Button : Zoom IN/OUT

when on FreeCamera
Number 1: no light
Number 2: white pointLight
Number 3: multicolor pointLight
Number 4: White spotLight
WASDQE-Movement
LControl- SpeedUp

*/
#pragma once
#include <glm\gtc\matrix_transform.hpp>
class CamController : public _NL::Core::CppScript
{
private:
	float BaseMovementSpeed = 3;
	float MovementSpeed = 3;
	float RotationSpeed = 1;
	_NL::Object::CameraObj_data::CamTransform* TCam;
	glm::vec2 mouseDelta;
	glm::vec2 oldMousePos;
public:
	_NL::Engine::WindowManager * W;
	_NL::Object::CameraObj* _this;
	void Start() override;
	void Update() override;
	void RotateCam();
	float TrackMouse();
	
	template<typename T>
	T lerp(T v0, T v1, float t) {
		return (1 - t) * v0 + t * v1;
	}
};

void CamController::Start() {
	TCam = &_this->Transform;
}

void CamController::Update() {

	TrackMouse();

	//SYNC WITH DELTA TIME
	float dts = W->Time.DeltaTime.asSeconds();
	
	//GET SIDE VECTOR
	glm::vec3 SIDEWAYS = glm::cross(TCam->rotation, TCam->LookAtCenter);

	//FORWARD-BACKWARDS MOVEMENT
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		TCam->position += TCam->LookAtCenter * MovementSpeed * dts;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		TCam->position -= TCam->LookAtCenter * MovementSpeed * dts;
	}
	//SIDEWAYS MOVEMENT
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
	
		TCam->position += SIDEWAYS * MovementSpeed * dts;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		TCam->position -= SIDEWAYS * MovementSpeed * dts;
	}
	//UP-DOWN MOVEMENT
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		TCam->position += TCam->rotation * MovementSpeed * dts;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		TCam->position -= TCam->rotation * MovementSpeed * dts;
	}
	//SPEED UP
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		MovementSpeed = lerp(MovementSpeed, 20.0f, dts);
	}
	else {
		MovementSpeed = lerp(MovementSpeed, 0.0f, dts*2);
	}
	//MOUSE BUTTONS CHANGE CAM FOV
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		_this->Settings.FOV += .01;
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
		_this->Settings.FOV -= .01;
	}

}



//CALCULATE WERE TO lOOK AT
void CamController::RotateCam() {
	glm::vec3 UPDOWN = glm::cross(TCam->LookAtCenter, TCam->rotation);
	glm::mat4 rotator = glm::rotate(glm::rotate(glm::mat4(), -mouseDelta.y*RotationSpeed / 100.0f, UPDOWN), -mouseDelta.x*RotationSpeed / 100.0f, TCam->rotation);

	TCam->LookAtCenter = glm::mat3(rotator) * TCam->LookAtCenter;
}

//TRACK MOUSE ON SCREEN
float CamController::TrackMouse() {
	glm::vec2 newMousePos((float)sf::Mouse::getPosition(*W->window).x, (float)sf::Mouse::getPosition(*W->window).y);
	mouseDelta = (newMousePos - oldMousePos);
	float moveSpeed = sqrt(mouseDelta.x*mouseDelta.x + mouseDelta.y*mouseDelta.y);
	if (moveSpeed != 0)
		RotateCam();
	oldMousePos = newMousePos;
	return moveSpeed;
}


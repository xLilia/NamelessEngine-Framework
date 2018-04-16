#pragma once
#include <glm\gtc\matrix_transform.hpp>

class CamController : public _NL::Core::CppScript
{
private:
	GLfloat BaseMovementSpeed = 3;
	GLfloat MovementSpeed = 3;
	GLfloat AxisSpeed = 1;
	glm::vec2 mouseDelta;
	glm::vec2 oldMousePos;
public:
	_NL::Engine::GameManager * W;
	_NL::Object::CameraObj* _this;
	void Start() override;
	void Update() override;
	void RotateCam();
	GLfloat TrackMouse();
	
	template<typename T>
	T lerp(T v0, T v1, GLfloat t) {
		return (1 - t) * v0 + t * v1;
	}
};

void CamController::Start() {

}

void CamController::Update() {

	TrackMouse();

	//SYNC WITH DELTA TIME
	GLfloat dts = W->GameTime.DeltaTime.asSeconds();
	
	//GET SIDE VECTOR
	glm::vec3 SIDEWAYS = glm::cross(_this->Axis, _this->LookAt);

	//FORWARD-BACKWARDS MOVEMENT
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		_this->Position += _this->LookAt * MovementSpeed * dts;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		_this->Position -= _this->LookAt * MovementSpeed * dts;
	}
	//SIDEWAYS MOVEMENT
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
	
		_this->Position += SIDEWAYS * MovementSpeed * dts;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		_this->Position -= SIDEWAYS * MovementSpeed * dts;
	}
	//UP-DOWN MOVEMENT
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		_this->Position += _this->Axis * MovementSpeed * dts;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		_this->Position -= _this->Axis * MovementSpeed * dts;
	}
	//SPEED UP
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
		MovementSpeed = lerp(MovementSpeed, 20.0f, dts);
	}
	else {
		MovementSpeed = lerp(MovementSpeed, BaseMovementSpeed, dts*2);
	}
	//MOUSE BUTTONS CHANGE CAM FOV
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		_this->FOV += 1;
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
		_this->FOV -= 1;
	}
	//LOAD NEXT SCENE
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
		std::cout << "Load next scene..." << std::endl;
		W->EndCurrentScene();
	}

	_this->updateAudioListenerWithCamTransform();
	
}

//CALCULATE WERE TO lOOK AT
void CamController::RotateCam() {
	glm::vec3 UPDOWN = glm::cross(_this->LookAt, _this->Axis);
	glm::mat4 rotator = glm::rotate(glm::rotate(glm::mat4(), -mouseDelta.y*AxisSpeed / 100.0f, UPDOWN), -mouseDelta.x*AxisSpeed / 100.0f, _this->Axis);

	_this->LookAt = glm::mat3(rotator) * _this->LookAt;
}

//TRACK MOUSE ON SCREEN
GLfloat CamController::TrackMouse() {
	glm::vec2 newMousePos((GLfloat)sf::Mouse::getPosition(*W->window).x, (GLfloat)sf::Mouse::getPosition(*W->window).y);
	mouseDelta = (newMousePos - oldMousePos);
	GLfloat moveSpeed = sqrt(mouseDelta.x*mouseDelta.x + mouseDelta.y*mouseDelta.y);
	if (moveSpeed != 0)
		RotateCam();
	oldMousePos = newMousePos;
	return moveSpeed;
}


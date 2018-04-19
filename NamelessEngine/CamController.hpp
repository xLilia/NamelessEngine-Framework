#pragma once	
class CamController : public _NL::Core::CppScript
{
private:
	GLfloat Drag = 10;
	glm::vec3 movementVector;
	GLfloat movementSpeed = 1;
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
	
};

void CamController::Start() {

}

void CamController::Update() {

	TrackMouse();

	//SYNC WITH DELTA TIME
	GLfloat dts = W->GameTime.DeltaTime.asSeconds();
	
	//GET SIDE VECTOR
	glm::vec3 SIDEWAYS = glm::cross(_this->UpAxis, _this->LookAt);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		movementVector += _this->LookAt * movementSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		movementVector -= _this->LookAt * movementSpeed;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		movementVector += SIDEWAYS * movementSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		movementVector -= SIDEWAYS * movementSpeed;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		movementVector += _this->UpAxis * movementSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		movementVector -= _this->UpAxis * movementSpeed;
	}

	movementVector = glm::lerp(movementVector, glm::vec3(0.0f), dts*Drag);

	_this->Position = glm::lerp(_this->Position, _this->Position + movementVector, dts);

	//SPEED UP
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
		movementSpeed = glm::lerp(movementSpeed, 10.0f, 1.0f);
	}
	else {
		movementSpeed = glm::lerp(movementSpeed, 1.0f, 1.0f);
	}

	//MOUSE BUTTONS CHANGE CAM FOV
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		_this->FOV += 1;
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
		_this->FOV -= 1;
	}

	//LOAD NEXT SCENE
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
		std::cout << "Load next scene..." << std::endl;
		W->EndCurrentScene();
	}

	_this->updateAudioListenerWithCamTransform();
	
}

//CALCULATE WERE TO lOOK AT
void CamController::RotateCam() {
	glm::vec3 UPDOWN = glm::cross(_this->LookAt, _this->UpAxis);
	glm::mat4 rotator = glm::rotate(glm::rotate(glm::mat4(), -mouseDelta.y*AxisSpeed / 100.0f, UPDOWN), -mouseDelta.x*AxisSpeed / 100.0f, _this->UpAxis);

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


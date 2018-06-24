#pragma once
class TestScript : public _NL::Core::Script<_NL::Object::GameObject>
{
private:
	_NL::Component::Transform* T;
	GLfloat x1 = 0;
	GLfloat y1 = 0;
	GLfloat z1 = 0;
	GLint Instcounter = 0;
	bool Move = false;

public:
	glm::vec3 target;
	_NL::Engine::NLManager * W;

	void Start() override;
	void Update() override;
};


void TestScript::Start(){
	T = _this->getComponent<_NL::Component::Transform>();
}

void TestScript::Update() {
	target = W->Cameras[0]->transformCam.Position;
	T = _this->getComponent<_NL::Component::Transform>();
	GLfloat t = W->GameTime.DeltaTime.asSeconds();
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::J) ) {
		glm::vec3 vec = target - T->transform.position;
		if (glm::length(vec) > 10) {

			T->transform.position = glm::lerp(T->transform.position, target, t / 2);
			if (_this->name == "Light1") {
				dynamic_cast<_NL::Object::LightObject*>(_this)->LightProperties.lightPosition = T->transform.position;
			}
			T->transform.QuaternionRotation = glm::slerp(T->transform.QuaternionRotation, T->LookAt(target, glm::vec3(0, 1, 0)), t / 2);
		}
		else {
			T->transform.position = glm::lerp(T->transform.position, glm::cross(T->transform.position-glm::normalize(vec),glm::normalize(vec)), t*2);
			T->transform.QuaternionRotation = glm::slerp(T->transform.QuaternionRotation, T->LookAt(target, glm::vec3(0, 1, 0)), t / 2);
		}
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
		x1 -= 0.1;
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
		x1 += 0.1;
	}
	else {
		x1 = glm::lerp(x1, 0.0f, t*2);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) {
		y1 -= 0.1;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
		y1 += 0.1;
	}
	else {
		y1 = glm::lerp(y1, 0.0f, t*2);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
		z1 -= 0.1;
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
		z1 += 0.1;
	}
	else {
		z1 = glm::lerp(z1, 0.0f, t*2);
	}

	x1 = glm::clamp(x1, -3.0f, 3.0f);
	y1 = glm::clamp(y1, -3.0f, 3.0f);
	z1 = glm::clamp(z1, -3.0f, 3.0f);
	T->EulerRotation(glm::vec3(x1, y1, z1)*t);
	

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
		T->transform.QuaternionRotation = glm::quat();
		T->transform.position = glm::vec3();
	}

}


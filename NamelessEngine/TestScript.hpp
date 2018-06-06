#pragma once
class TestScript : public _NL::Core::Script
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
	_NL::Engine::GameManager * W;

	void Start() override;
	void Update() override;
};


void TestScript::Start(){
	//target = W->Cameras[0]->Position;
	T = _this->getComponent<_NL::Component::Transform>();
	//T->transform.scale *= 0.1;
}

void TestScript::Update() {
	target = W->Cameras[0]->transformCam.Position;
	T = _this->getComponent<_NL::Component::Transform>();
	GLfloat t = W->GameTime.DeltaTime.asSeconds();
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add) ) {
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
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8)) {
		x1 -= 0.1;
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5)) {
		x1 += 0.1;
	}
	else {
		x1 = glm::lerp(x1, 0.0f, t*2);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad9)) {
		y1 -= 0.1;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad7)) {
		y1 += 0.1;
	}
	else {
		y1 = glm::lerp(y1, 0.0f, t*2);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6)) {
		z1 -= 0.1;
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4)) {
		z1 += 0.1;
	}
	else {
		z1 = glm::lerp(z1, 0.0f, t*2);
	}

	x1 = glm::clamp(x1, -3.0f, 3.0f);
	y1 = glm::clamp(y1, -3.0f, 3.0f);
	z1 = glm::clamp(z1, -3.0f, 3.0f);
	T->RotateEuler(glm::vec3(x1, y1, z1)*t);
	

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
			W->SafeKillObj(W->Lights[0]);
		//_NL::Object::GameObject* I = W->SafeInstantiateObj<_NL::Object::GameObject>(_this);
		//I->name = "panqueca";
		//I->getComponent<_NL::Component::Transform>()->transform.position = T->transform.position + glm::vec3(rand() % 100, rand() % 100, rand() % 100);
		//I->getComponent<_NL::Component::Transform>()->transform.QuaternionRotation = T->transform.QuaternionRotation;
		//I->getComponent<_NL::Component::Transform>()->transform.scale = T->transform.scale;
	}
	
}


#pragma once
class TemplateScript : public _NL::Core::CppScript
{
private:
	_NL::Component::Transform* T;
	GLfloat x1 = 0;
	GLfloat y1 = 0;
	GLfloat z1 = 0;
	GLint Instcounter = 0;
	bool Move = false;

public:
	glm::vec3* target;
	_NL::Engine::GameManager * W;
	_NL::Object::GameObject* _this;
	void Start() override;
	void Update() override;
};

void TemplateScript::Start(){
	if (!target) target = new glm::vec3();
	T = _this->getComponent<_NL::Component::Transform>();
}

void TemplateScript::Update() {
	GLfloat t = W->GameTime.DeltaTime.asSeconds();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2) ) {
		T->transform.QuaternionRotation = glm::slerp(T->transform.QuaternionRotation, T->LookAt(*target, glm::vec3(0, 1, 0)),t);
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
	

	//T->RotateEuler(glm::vec3(s1, 0, 0)*t);
	
	//T->transform.QuaternionRotation = glm::lerp(T->transform.QuaternionRotation, T->transform.QuaternionRotation + glm::quat(s1, 0, 1, 0), t);

	//s1 += 0.0001f;
	//T->EulerRotation(0, glm::cos(s1)*t, 0);

	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)) {
	//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
	//		W->RenderExposure += 0.01;
	//	}
	//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
	//		W->RenderExposure -= 0.01;
	//	}
	//}
	//
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
	//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
	//		W->RenderGamma += 0.01;
	//	}
	//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
	//		W->RenderGamma -= 0.01;
	//	}
	//}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
		W->CurrentScene->Instantiate(_this, T->transform.position + glm::vec3(rand() % 100, rand() % 100, rand() % 100), T->transform.QuaternionRotation, T->transform.scale);
	}
	
}

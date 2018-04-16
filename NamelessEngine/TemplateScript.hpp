#pragma once
class TemplateScript : public _NL::Core::CppScript
{
private:
	_NL::Component::Transform* T;
	GLfloat s1 = 0;
	GLint Instcounter = 0;
	bool Move = false;
public:
	_NL::Engine::GameManager * W;
	_NL::Object::GameObject* _this;
	void Start() override;
	void Update() override;
};

void TemplateScript::Start(){
	T = _this->getComponent<_NL::Component::Transform>();
}

void TemplateScript::Update() {
	GLfloat t = W->GameTime.DeltaTime.asSeconds();


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ) {
		//T->transform.position += W->Time.DeltaTime.asSeconds() * glm::vec3(std::rand()/10000, std::rand() / 10000, std::rand() / 10000);
		//T->transform.position -= W->Time.DeltaTime.asSeconds() * glm::vec3(std::rand() / 10000, std::rand() / 10000, std::rand() / 10000);
		s1 += 0.0001f;
		T->EulerRotation(0, glm::cos(s1)*t, 0);
	
		//std::cout << s;
		//T->transform.position.y += glm::cos(s);
	}

	s1 += 0.0001f;
	T->EulerRotation(0, glm::cos(s1)*t, 0);

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
		W->CurrentScene->Instantiate(_this, T->transform.position + glm::vec3(rand() % 100, rand() % 100, rand() % 100), T->transform.EulerRotation, T->transform.scale);
	}
	
}

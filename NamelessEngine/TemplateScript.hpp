#pragma once
class TemplateScript : public _NL::Core::CppScript
{
private:
	_NL::Component::Transform* T;
	float s = 0;
	bool Move = false;
public:
	_NL::Engine::WindowManager * W;
	_NL::Object::GameObject* _this;
	void Start() override;
	void Update() override;
};

void TemplateScript::Start(){
	T = _this->getComponent<_NL::Component::Transform>();
}

void TemplateScript::Update() {

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ) {
		T->transform.position += W->Time.DeltaTime.asSeconds() * glm::vec3(std::rand()/10000, std::rand() / 10000, std::rand() / 10000);
		T->transform.position -= W->Time.DeltaTime.asSeconds() * glm::vec3(std::rand() / 10000, std::rand() / 10000, std::rand() / 10000);

		s += 0.0001f;
		//std::cout << s;
		T->transform.position.y += s;
	}
}
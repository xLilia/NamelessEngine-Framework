#pragma once
#include "NL.hpp"

class TemplateScript : public _NL::Core::CppScript
{
private:
	_NL::Component::Transform* T;
public:
	_NL::Engine::Time* Time;
	_NL::Object::GameObject* _this;
	void Start() override;
	void Update() override;
};

void TemplateScript::Start(){
	T = _this->getComponent(_NL::Component::Transform());
	T->transform.position.z += -1;
}

void TemplateScript::Update() {
	T->transform.position.y = std::sin(Time->Clock.getElapsedTime().asSeconds());

}
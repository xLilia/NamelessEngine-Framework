#pragma once
class TemplateScript : public _NL::Core::CppScript
{
private:
	_NL::Component::Transform* T;
	int i = 0;
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
	if (W->Event.type == W->Event.KeyPressed) {
	
	}

	if (W->Event.text.unicode == int32_t('w') ) {
		T->transform.position.z += W->Time.DeltaTime.asSeconds();
	}
}
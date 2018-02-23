#pragma once
class CamController : public _NL::Core::CppScript
{
private:

public:
	_NL::Engine::WindowManager * W;
	_NL::Object::CameraObj* _this;
	void Start() override;
	void Update() override;
};

void CamController::Start() {
	
}

void CamController::Update() {
	if (W->Event.type == W->Event.KeyPressed) {

	}

	if (W->Event.text.unicode == int32_t('w')) {
		_this->Transform.position.z += W->Time.DeltaTime.asSeconds()*2;
	}
	if (W->Event.text.unicode == int32_t('s')) {
		_this->Transform.position.z -= W->Time.DeltaTime.asSeconds()*2;
	}
	if (W->Event.text.unicode == int32_t('a')) {
		_this->Transform.position.x += W->Time.DeltaTime.asSeconds()*2;
	}
	if (W->Event.text.unicode == int32_t('d')) {
		_this->Transform.position.x -= W->Time.DeltaTime.asSeconds()*2;
	}
	if (W->Event.text.unicode == int32_t('e')) {
		_this->Transform.position.y += W->Time.DeltaTime.asSeconds() * 2;
	}
	if (W->Event.text.unicode == int32_t('q')) {
		_this->Transform.position.y -= W->Time.DeltaTime.asSeconds() * 2;
	}
}
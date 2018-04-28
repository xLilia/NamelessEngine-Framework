#pragma once
class ParticleScript : public _NL::Core::Script
{
private:
	_NL::Component::Transform* T;
public:
	_NL::Engine::GameManager * W;
	void Start() override;
	void Update() override;
};


void ParticleScript::Start(){
	T = _this->getComponent<_NL::Component::Transform>();
	//T->transform.scale *= 0.1;
}

void ParticleScript::Update() {
	T = _this->getComponent<_NL::Component::Transform>();
	GLfloat t = W->GameTime.DeltaTime.asSeconds();

	T->transform.position.y += t*5;
}


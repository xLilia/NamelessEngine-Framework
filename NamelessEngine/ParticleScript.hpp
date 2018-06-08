#pragma once
class ParticleScript : public _NL::Core::Script<_NL::Object::ParticleObj>
{
private:
	_NL::Component::Transform* T;
public:
	_NL::Engine::NLManager * W;
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
	
	GLfloat LO;
	GLfloat HI;
	GLfloat RandomX;
	GLfloat RandomY;
	GLfloat RandomZ;


	LO = -1;
	HI = 1;
	RandomX = LO + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX / (HI - LO)));
	RandomY = LO + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX / (HI - LO)));
	RandomZ = LO + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX / (HI - LO)));


	glm::vec3 rv(RandomX, RandomY, RandomZ);

	T->transform.QuaternionRotation = T->LookAt(W->Cameras[0]->transformCam.Position);
	
	T->transform.position = glm::lerp(T->transform.position, T->transform.position + rv,t);



	//T->transform.position.y += t*5;
}


#pragma once
#include "BOIDObject.h"

class BOIDscript : public _NL::Core::Script<BOIDObject>
{
private:

public:
	_NL::Engine::NLManager* W;
	float FleeDist = 1.0f;
	float AlignRatio = 8.0f;

	float CohesionMult = 0.6f;
	float SeparatingMult = 0.1f;
	float AlignMult = 1.0f;
	float followMult = 0.0f;

	int pulse = 0;
	int pulsereset = 0;

	void Start() override;
	void Update() override;

	glm::vec3 ExeBoidRules(std::vector<BOIDObject*>& bl);
};

glm::vec3 BOIDscript::ExeBoidRules(std::vector<BOIDObject*>& bl) {

	glm::vec3 CohesionVector;
	glm::vec3 SeparatingVector;
	glm::vec3 AlignVector;
	glm::vec3 followVector;

	glm::vec3 AverageVel;
	glm::vec3 cm;

	//pulse--;
	//if(pulse==0)
	for (int b = 0; b < bl.size(); b++) {
		//r1_Cohesing(); 
		//fly towards the centre of mass of neighbouring boids.
		if (bl[b] != _this) {
			cm += bl[b]->T->transform.position;
		}

		//r2_Separating();
		//keep a small distance away from other objects (including other boids)
		glm::vec3 fleeVect = bl[b]->T->transform.position - _this->T->transform.position;
		if (glm::length(fleeVect) < FleeDist) {
			SeparatingVector -= fleeVect;
		}

		//r3_Aligning();
		//Boids try to match velocity with near boids.
		if (bl[b] != _this) {
			AverageVel += bl[b]->velocity;
		}

		
		//r4_folowvec();
		//if (length(followVector) == 0) followVector = glm::vec3(INFINITY);
		//if (bl[b] != _this) {
		//	if (length(fleeVect) < length(followVector)) {
		//		followVector = fleeVect;
		//	}
		//}
		followVector = _this->velocity;

		pulse = pulsereset;
	}

	cm /= bl.size() - 1; //MEDIAN POINT
	CohesionVector = (cm - _this->T->transform.position) / 100.0f; //1% of speed


	AverageVel /= bl.size() - 1;
	AlignVector = (AverageVel - _this->velocity) / AlignRatio;

	CohesionVector *= CohesionMult;
	SeparatingVector *= SeparatingMult;
	AlignVector *= AlignMult;
	followVector *= followMult;

	return CohesionVector + SeparatingVector + AlignVector + followVector;
}

void BOIDscript::Start() {

}

void BOIDscript::Update() {

	std::vector<BOIDObject*> Blist = W->CurrentScene->getAllObjsOfType<BOIDObject>(_this->getTypeName());

	_this->velocity += ExeBoidRules(Blist);
	_this->T->transform.position += _this->velocity;
	_this->T->transform.QuaternionRotation = _this->T->LookAt(_this->velocity,0,-1,0);


	if (_this->T->transform.position.x > 10) {
		_this->T->transform.position.x = -10;
	}

	if (_this->T->transform.position.y > 20) {
		_this->T->transform.position.y = 0;
	}

	if (_this->T->transform.position.z > -30) {
		_this->T->transform.position.z = -50;
	}


	if (_this->T->transform.position.x < -10) {
		_this->T->transform.position.x = 10;
	}

	if (_this->T->transform.position.y < 0) {
		_this->T->transform.position.y = 20;
	}

	if (_this->T->transform.position.z < -50) {
		_this->T->transform.position.z = -30;
	}

	if (length(_this->velocity) > .1f) {
		_this->velocity = glm::normalize(_this->velocity) * .1f;
	}

	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) 
	//	W->CurrentScene->Instantiate<_NL::Object::GameObject>(_this);

}


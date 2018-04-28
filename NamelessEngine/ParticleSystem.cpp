#include "ParticleSystem.h"



_NL::Object::ParticleSystem::ParticleSystem()
{
}

void _NL::Object::ParticleSystem::ON()
{
	ON_OFF = true;
}

void _NL::Object::ParticleSystem::OFF()
{
	ON_OFF = false;
}

glm::vec3 _NL::Object::ParticleSystem::coneClamp(glm::vec3 pos) {

	GLfloat RadiusBase = SpawnerTransform.SpawnerRadius * SpawnerTransform.SpawnerConeRadiusIncrement;
	GLfloat InvRadiusBase = SpawnerTransform.SpawnerRadius - RadiusBase;
	GLfloat InvNewHeightRadius = (pos.y * InvRadiusBase) / (SpawnerTransform.SpawnerHeight);
	GLfloat NewHeightRadius = SpawnerTransform.SpawnerRadius - InvNewHeightRadius;

	if (pos.x > NewHeightRadius){
		pos.x = NewHeightRadius;
	}
	else 
	if (pos.x < -NewHeightRadius) {
		pos.x = -NewHeightRadius;
	}

	if (pos.z > NewHeightRadius) {
		pos.z = NewHeightRadius;
	}
	else 
	if (pos.z < -NewHeightRadius) {
		pos.z = -NewHeightRadius;
	}
	return pos;
}

glm::quat _NL::Object::ParticleSystem::getDirectionQuaternion(glm::vec3 dir, glm::vec3 newdir)
{
	//DIRECTION QUATERNION
	GLfloat dot = glm::dot(glm::normalize(dir), glm::normalize(newdir));
	GLfloat Angle = glm::acos(dot);
	glm::vec3 rotationAxis = glm::normalize(glm::cross(glm::normalize(dir), glm::normalize(newdir)));
	glm::quat DirQuat = glm::toQuat(glm::rotate(glm::mat4(), Angle, -rotationAxis));

	return DirQuat;
}

void _NL::Object::ParticleSystem::TickSystem()
{
	if (ON_OFF) {
		SpawnItTime += TimeScale->DeltaTime.asSeconds();

		if (SpawnItTime >= SpawnRate) {
			SpawnItTime = 0;
			//CALCULATE NEW RANDOM POINT IN SPAWN CILINDER
			glm::vec3 newSpawnPoint(0,0,0);

			GLfloat LO; 
			GLfloat HI;

			LO = -SpawnerTransform.SpawnerRadius;
			HI = SpawnerTransform.SpawnerRadius;
			GLfloat RandomRadiusX = LO + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX / (HI - LO)));
			GLfloat RandomRadiusZ = LO + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX / (HI - LO)));
			newSpawnPoint += glm::vec3(RandomRadiusX, 0, RandomRadiusZ);

			LO = 0;
			HI = SpawnerTransform.SpawnerHeight;
			GLfloat RandomHeight = LO + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX / (HI - LO)));
			newSpawnPoint += glm::vec3(0, RandomHeight, 0);
			
			//CLAMP TO SPAWN CONE
			newSpawnPoint = coneClamp(newSpawnPoint);

			glm::quat quaternionDir = getDirectionQuaternion(newSpawnPoint, SpawnerTransform.Direction);

			newSpawnPoint = quaternionDir * newSpawnPoint;

			newSpawnPoint += SpawnerTransform.Position;

			if (Particle->LookAtCamera == false) {
				ActiveParticles.push_back(
					static_cast<_NL::Object::ParticleObj*>(
						CurrentScene->Instantiate(Particle, newSpawnPoint, quaternionDir, SpawnerTransform.Scale)
						)
				);
			}
			
		}
		for each (_NL::Object::ParticleObj* P in ActiveParticles)
		{
			P->lifeTime -= TimeScale->DeltaTime.asSeconds();
			if (P->lifeTime <= 0) {
				P->Alive = false;
			}
			//UPDATE BEHAVIOUR
			ParticlesBehavior->_this = P;
			if (!ParticlesBehavior->awake) {
				ParticlesBehavior->awake = true;
				ParticlesBehavior->Start();
			}
			if (P->Alive) {
				ParticlesBehavior->Update();
			}
			else {
				ParticlesBehavior->End();
				//ActiveParticles.erase( std::remove(ActiveParticles.begin(), ActiveParticles.end(), P), ActiveParticles.end());
				//ActiveParticles.erase(ActiveParticles.begin(), ActiveParticles.end() + Pn);
				//ActiveParticles.shrink_to_fit();
			}
		}
	}
}

char * _NL::Object::ParticleSystem::ClassName() const
{
	return "_NL::Object::ParticleSystem";
}

_NL::Object::ParticleSystem::~ParticleSystem()
{
}

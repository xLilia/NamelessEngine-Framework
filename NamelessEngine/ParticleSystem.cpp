#include "ParticleSystem.h"



_NL::Object::ParticleSystem::ParticleSystem()
{
	SPAWN_ON();
	BEHAVIOUR_ON();
}

void _NL::Object::ParticleSystem::SPAWN_ON()
{
	ON_OFF = true;
}

void _NL::Object::ParticleSystem::SPAWN_OFF()
{
	ON_OFF = false;
}

void _NL::Object::ParticleSystem::BEHAVIOUR_ON()
{
	BON_BOFF = true;
}

void _NL::Object::ParticleSystem::BEHAVIOUR_OFF()
{
	BON_BOFF = false;
}

glm::vec3 _NL::Object::ParticleSystem::coneClamp(glm::vec3 pos) {

	GLfloat RadiusBase = SpawnerTransform.SpawnerRadius * SpawnerTransform.SpawnerConeVertexRadius;
	GLfloat InvRadiusBase = SpawnerTransform.SpawnerRadius - RadiusBase;
	GLfloat InvNewHeightRadius = (pos.y * InvRadiusBase) / (SpawnerTransform.SpawnerHeight);
	GLfloat NewHeightRadius = SpawnerTransform.SpawnerRadius - InvNewHeightRadius;


	glm::vec3 checkpos = glm::vec3(pos.x, 0, pos.z);

	if (glm::length(checkpos) > NewHeightRadius) {
		checkpos = glm::normalize(checkpos)*NewHeightRadius;
	}
	else
	if (glm::length(checkpos) < -NewHeightRadius) {
		checkpos = glm::normalize(checkpos)*-NewHeightRadius;
	}

	pos.x = checkpos.x;
	pos.z = checkpos.z;

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
		SpawnItTime += TimeScale->Clock.getElapsedTime().asSeconds();

		if (SpawnItTime >= SpawnRate) {
			SpawnItTime = 0;
			//CALCULATE NEW RANDOM POINT IN SPAWN CILINDER
			glm::vec3 newSpawnPoint(0,0,0);
			GLfloat LO;
			GLfloat HI;
			GLfloat RandomX;
			GLfloat RandomY;
			GLfloat RandomZ;
			if (SpawnerTransform.PM == PARTICLE_SPAWN_MODE::SPHERE) {
				LO = -SpawnerTransform.SpawnerRadius;
				HI = SpawnerTransform.SpawnerRadius;
				RandomX = LO + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX / (HI - LO)));
				RandomY = LO + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX / (HI - LO)));
				RandomZ = LO + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX / (HI - LO)));
				GLfloat RandomR = LO + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX / (HI - LO)));
				
				newSpawnPoint = glm::normalize(glm::vec3(RandomX, RandomY, RandomZ)) * RandomR;
				newSpawnPoint += SpawnerTransform.Position;
			}

			if (SpawnerTransform.PM == PARTICLE_SPAWN_MODE::CONE) {
				
				LO = -SpawnerTransform.SpawnerRadius;
				HI = SpawnerTransform.SpawnerRadius;

				RandomX = LO + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX / (HI - LO)));
				RandomZ = LO + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX / (HI - LO)));

				LO = 0;
				HI = SpawnerTransform.SpawnerHeight;
				RandomY = LO + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX / (HI - LO)));
				newSpawnPoint = glm::vec3(RandomX, RandomY, RandomZ);

				//CLAMP TO SPAWN CONE
				newSpawnPoint = -coneClamp(newSpawnPoint);
				newSpawnPoint += SpawnerTransform.Position * 2.0f;
			}

			//glm::quat quaternionDir = getDirectionQuaternion(newSpawnPoint, -SpawnerTransform.Direction);
			//
			//newSpawnPoint = quaternionDir * newSpawnPoint;

			ActiveParticles.push_back(static_cast<_NL::Object::ParticleObj*>(
				CurrentScene->Instantiate(
					Particle, newSpawnPoint, glm::quat(), SpawnerTransform.Scale)
				)
			);
			//COPY DEFAULT PARTICLE
			ActiveParticles[ActiveParticles.size() - 1]->lifeTime = Particle->lifeTime;
			ActiveParticles[ActiveParticles.size() - 1]->Awake = Particle->Awake;
			ActiveParticles[ActiveParticles.size() - 1]->Alive = Particle->Alive;
			
		}

		for (int i = 0; i < ActiveParticles.size(); i++) {
			ActiveParticles[i]->lifeTime -= TimeScale->DeltaTime.asSeconds();
			if (ActiveParticles[i]->lifeTime <= 0) {
				ActiveParticles[i]->Alive = false;
			}
			//UPDATE BEHAVIOUR
			ParticlesBehavior->_this = ActiveParticles[i];
			if (BON_BOFF) {
				if (!ActiveParticles[i]->Awake) {
					ActiveParticles[i]->Awake = true;
					ParticlesBehavior->Start();
				}
			}
			if (ActiveParticles[i]->Alive) {
				if (BON_BOFF) ParticlesBehavior->Update();
			}
			else { //KILL PARTICLE
				if (BON_BOFF) ParticlesBehavior->End();
				CurrentScene->KillObjectInstance(ActiveParticles[i]);
				ActiveParticles.erase(std::remove(ActiveParticles.begin(), ActiveParticles.end(), ActiveParticles[i]), ActiveParticles.end());
				
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

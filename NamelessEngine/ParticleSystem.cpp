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

	//GLfloat RadiusBase = SpawnerTransform.SpawnerConeBaseRadius * SpawnerTransform.SpawnerConeVertexRadius;
	//GLfloat InvRadiusBase = SpawnerTransform.SpawnerConeBaseRadius - RadiusBase;
	//GLfloat InvNewHeightRadius = (pos.y * InvRadiusBase) / (SpawnerTransform.SpawnerHeightY);
	//GLfloat NewHeightRadius = SpawnerTransform.SpawnerConeBaseRadius - InvNewHeightRadius;

	GLfloat RadiusBaseX = SpawnerTransform.SpawnerWidthX * SpawnerTransform.SpawnerConeVertexRadius;
	GLfloat InvRadiusBaseX = SpawnerTransform.SpawnerWidthX - RadiusBaseX;
	GLfloat InvNewHeightRadiusX = (pos.y * InvRadiusBaseX) / (SpawnerTransform.SpawnerHeightY);
	GLfloat NewHeightRadiusX = SpawnerTransform.SpawnerWidthX - InvNewHeightRadiusX;
	
	GLfloat RadiusBaseZ = SpawnerTransform.SpawnerWidthZ * SpawnerTransform.SpawnerConeVertexRadius;
	GLfloat InvRadiusBaseZ = SpawnerTransform.SpawnerWidthZ - RadiusBaseZ;
	GLfloat InvNewHeightRadiusZ = (pos.y * InvRadiusBaseZ) / (SpawnerTransform.SpawnerHeightY);
	GLfloat NewHeightRadiusZ = SpawnerTransform.SpawnerWidthZ - InvNewHeightRadiusZ;

	glm::vec3 checkpos = glm::vec3(pos.x, 0, pos.z);
	glm::vec3 clampC = glm::vec3(NewHeightRadiusX, 0, NewHeightRadiusZ);

	//if (glm::length(checkpos) > NewHeightRadius) {
	//	checkpos = glm::normalize(checkpos)*NewHeightRadius;
	//} 
	//else
	//if (glm::length(checkpos) < -NewHeightRadius) {
	//	checkpos = glm::normalize(checkpos)*-NewHeightRadius;
	//}

	//if (glm::length(checkpos) > glm::length(clampC)) {
	//	checkpos = glm::normalize(checkpos)*clampC;
	//} 
	//else
	//if (glm::length(checkpos) < -glm::length(clampC)) {
	//	checkpos = glm::normalize(checkpos)*-clampC;
	//}

	if (checkpos.x > clampC.x) {
		checkpos.x = clampC.x;
	}else
	if(checkpos.x < -clampC.x) {
		checkpos.x = -clampC.x;
	}
	
	if (checkpos.z > clampC.z) {
		checkpos.z = clampC.z;
	}			
	else		
	if (checkpos.z < -clampC.z) {
		checkpos.z = -clampC.z;
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

		if (SpawnPerFrame < 1.0) {
			SpawnN += SpawnPerFrame;
		}
		else {
			SpawnN = SpawnPerFrame;
		}

		if (SpawnN >= 1.0) {
			
			
			GLuint oldSize = ActiveParticles.size();
			//ActiveParticles.reserve(ActiveParticles.capacity() + SpawnPerFrame);
			ActiveParticles.resize(ActiveParticles.size() + (int)SpawnN);
			//CurrentScene->ObjectList.resize(CurrentScene->ObjectList.size() + SpawnPerFrame);

			for (int spf = 0; spf < (int)SpawnN; spf++) {
				//CALCULATE NEW RANDOM POINT 

				glm::vec3 newSpawnPoint(0, 0, 0);
				GLfloat LO;
				GLfloat HI;
				GLfloat RandomX;
				GLfloat RandomY;
				GLfloat RandomZ;

				LO = -SpawnerTransform.SpawnerWidthX;
				HI = SpawnerTransform.SpawnerWidthX;
				RandomX = LO + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX / (HI - LO)));

				LO = -SpawnerTransform.SpawnerHeightY;
				HI = SpawnerTransform.SpawnerHeightY;
				RandomY = LO + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX / (HI - LO)));

				LO = -SpawnerTransform.SpawnerWidthZ;
				HI = SpawnerTransform.SpawnerWidthZ;
				RandomZ = LO + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX / (HI - LO)));


				if (SpawnerTransform.SpawnerShape == PARTICLE_SPAWN_MODE::CUBE) {

					newSpawnPoint = glm::vec3(RandomX, RandomY, RandomZ);
					newSpawnPoint += SpawnerTransform.Position;
				}

				if (SpawnerTransform.SpawnerShape == PARTICLE_SPAWN_MODE::SPHERE) {

					//LO = -SpawnerTransform.SpawnerHeightY;
					//HI = SpawnerTransform.SpawnerHeightY;
					//RandomX = LO + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX / (HI - LO)));
					//RandomY = LO + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX / (HI - LO)));
					//RandomZ = LO + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX / (HI - LO)));

					glm::vec3 RandomR(0);

					LO = -SpawnerTransform.SpawnerWidthX;
					HI = SpawnerTransform.SpawnerWidthX;
					RandomR.x = LO + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX / (HI - LO)));

					LO = -SpawnerTransform.SpawnerHeightY;
					HI = SpawnerTransform.SpawnerHeightY;
					RandomR.y = LO + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX / (HI - LO)));

					LO = -SpawnerTransform.SpawnerWidthZ;
					HI = SpawnerTransform.SpawnerWidthZ;
					RandomR.z = LO + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX / (HI - LO)));

					newSpawnPoint = glm::normalize(glm::vec3(RandomX, RandomY, RandomZ)) * RandomR;
					newSpawnPoint += SpawnerTransform.Position;
				}

				if (SpawnerTransform.SpawnerShape == PARTICLE_SPAWN_MODE::CONE) {

					//CLAMP TO SPAWN CONE
					newSpawnPoint = -coneClamp(glm::vec3(RandomX, RandomY, RandomZ));
					newSpawnPoint += SpawnerTransform.Position * 2.0f;
				}

				//glm::quat quaternionDir = getDirectionQuaternion(newSpawnPoint, -SpawnerTransform.Direction);
				//
				//newSpawnPoint = quaternionDir * newSpawnPoint;

				_NL::Object::ParticleObj* SpawnParticle = new _NL::Object::ParticleObj(*Particle);

				if (ActiveParticles.size() == 1) {
					SpawnParticle->getComponent<_NL::Component::Transform>()->transform.position = newSpawnPoint;
					SpawnParticle->getComponent<_NL::Component::Transform>()->transform.scale = SpawnerTransform.Scale;
					ActiveParticles[oldSize + spf] = SpawnParticle;

					CurrentScene->addObjectToWorld(SpawnParticle);
				}
				else {
					ActiveParticles[oldSize + spf] = CurrentScene->Instantiate<_NL::Object::ParticleObj>(SpawnParticle);
					ActiveParticles[oldSize + spf]->getComponent<_NL::Component::Transform>()->transform.position = newSpawnPoint;
					ActiveParticles[oldSize + spf]->getComponent<_NL::Component::Transform>()->transform.scale = SpawnerTransform.Scale;
				}

				//COPY DEFAULT PARTICLE
				ActiveParticles[oldSize + spf]->lifeTime = Particle->lifeTime;
				ActiveParticles[oldSize + spf]->Awake = Particle->Awake;
				ActiveParticles[oldSize + spf]->Alive = Particle->Alive;
			}
			SpawnN = 0;
		}

		for (int i = 0; i < ActiveParticles.size(); i++) {
			
			ActiveParticles[i]->lifeTime -= TimeScale->DeltaTime.asSeconds();

			if (ActiveParticles[i]->lifeTime <= 0) {
				ActiveParticles[i]->Alive = false;
			}
			//UPDATE BEHAVIOUR
			if (ParticlesBehavior != NULL && BON_BOFF == true) {
				
				ParticlesBehavior->_this = ActiveParticles[i];
				
				if (!ActiveParticles[i]->Awake) {
					ActiveParticles[i]->Awake = true;
					ParticlesBehavior->Start();
				}
				
				if (ActiveParticles[i]->Alive) {
					ParticlesBehavior->Update();
				}
				else { //KILL PARTICLE
					ParticlesBehavior->End();
					CurrentScene->KillObjectInstance(ActiveParticles[i]);
					ActiveParticles.erase(ActiveParticles.begin() + i);
				}
			}
			else {
				if (!ActiveParticles[i]->Alive) {
					//KILL PARTICLE
					CurrentScene->KillObjectInstance(ActiveParticles[i]);
					ActiveParticles.erase(ActiveParticles.begin() + i);
				}
			}
		}
	}
}

char * _NL::Object::ParticleSystem::getTypeName() const
{
	return "_NL::Object::ParticleSystem";
}

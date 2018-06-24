#pragma once
#include "NamelessCore.hpp"
#include "ParticleObj.h"
#include "WorldSpace.h"
#include "NL_Time.h"
namespace _NL {
	namespace Object {
		class ParticleSystem : public _NL::Core::Object
		{
		public:
			enum PARTICLE_SPAWN_MODE {
				CUBE = 0,
				SPHERE = 1,
				CONE = 2	
			};
			ParticleSystem();
			std::vector<_NL::Object::ParticleObj*> ActiveParticles;
			_NL::Core::Script<_NL::Object::ParticleObj>* ParticlesBehavior = NULL;
			_NL::Object::ParticleObj* Particle = NULL;
			_NL::Engine::WorldSpace* CurrentScene = NULL;
			_NL::Engine::Time* TimeScale = NULL;
			GLfloat SpawnPerFrame = 1;
			struct SpwTransform {
				glm::vec3 Position = glm::vec3(0, 0, 0);
				glm::vec3 Direction = glm::vec3(0, 1, 0);
				glm::vec3 Scale = glm::vec3(1, 1, 1);
				GLfloat SpawnerHeightY = 1;
				GLfloat SpawnerWidthX = 1;
				GLfloat SpawnerWidthZ = 1;
				GLfloat SpawnerConeVertexRadius = .5;
				PARTICLE_SPAWN_MODE SpawnerShape = PARTICLE_SPAWN_MODE::CUBE;
			} SpawnerTransform;
			glm::vec3 coneClamp(glm::vec3 pos);
			glm::quat getDirectionQuaternion(glm::vec3 dir, glm::vec3 newdir);
			void SPAWN_ON();
			void SPAWN_OFF();
			void BEHAVIOUR_ON();
			void BEHAVIOUR_OFF();
			void TickSystem();
			char* getTypeName() override;
		private:
			GLfloat SpawnN = 0;
			bool ON_OFF;
			bool BON_BOFF;
		};
	}
}

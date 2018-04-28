#pragma once
#include "NamelessCore.h"
#include "ParticleObj.h"
#include "WorldSpace.h"
#include "NL_Time.h"
namespace _NL {
	namespace Object {
		class ParticleSystem : public _NL::Core::Object
		{
		public:
			enum PARTICLE_SPAWN_MODE {
				SPHERE = 0,
				CONE = 1	
			};
			ParticleSystem();
			~ParticleSystem();
			std::vector<_NL::Object::ParticleObj*> ActiveParticles;
			_NL::Core::Script* ParticlesBehavior;
			_NL::Object::ParticleObj* Particle;
			_NL::Engine::WorldSpace* CurrentScene = NULL;
			_NL::Engine::Time* TimeScale;
			GLfloat SpawnRate = 1;
			struct SpwTransform {
				glm::vec3 Position = glm::vec3(0, 0, 0);
				glm::vec3 Direction = glm::vec3(0, 1, 0);
				glm::vec3 Scale = glm::vec3(1, 1, 1);
				GLfloat SpawnerRadius = 1;
				GLfloat SpawnerConeVertexRadius = .5;
				GLfloat SpawnerHeight = 1;
				PARTICLE_SPAWN_MODE PM = PARTICLE_SPAWN_MODE::SPHERE;
			} SpawnerTransform;
			glm::vec3 coneClamp(glm::vec3 pos);
			glm::quat getDirectionQuaternion(glm::vec3 dir, glm::vec3 newdir);
			void SPAWN_ON();
			void SPAWN_OFF();
			void BEHAVIOUR_ON();
			void BEHAVIOUR_OFF();
			void TickSystem();
			char* ClassName() const override;
		private:
			GLfloat SpawnItTime = 0;
			bool ON_OFF;
			bool BON_BOFF;
		};
	}
}

#pragma once
#include "NamelessCore.h"
#include "Transform.h"
#include "MeshRenderer.h"

namespace _NL{
	namespace Object {
		class ParticleObj : public _NL::Core::Object
		{
		public:
			ParticleObj(_NL::Component::MeshRenderer* PMesh);
			~ParticleObj();
			GLfloat lifeTime;
			bool Awake;
			bool Alive;
			char* ClassName() const override;
		
		};
	}
}


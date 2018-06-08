#pragma once
#include "NamelessCore.hpp"
#include "Transform.h"
#include "MeshRenderer.h"

namespace _NL{
	namespace Object {
		class ParticleObj : public _NL::Core::Object
		{
		public:
			ParticleObj();
			GLfloat lifeTime;
			bool Awake;
			bool Alive;
			virtual char* getTypeName() const override;
		};
	}
}


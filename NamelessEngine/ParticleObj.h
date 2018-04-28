#pragma once
#include "NamelessCore.h"
#include "Transform.h"
namespace _NL{
	namespace Object {
		class ParticleObj : public _NL::Core::Object
		{
		public:
			ParticleObj();
			~ParticleObj();

			GLfloat lifeTime = 1;
			bool LookAtCamera = false;
			bool Alive = true;
			char* ClassName() const override;
		
		};
	}
}


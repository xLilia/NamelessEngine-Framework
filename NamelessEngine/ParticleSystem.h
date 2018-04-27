#pragma once
#include "NamelessCore.h"
namespace _NL {
	namespace Component {
		class ParticleSystem : public _NL::Core::Component
		{
		public:
			ParticleSystem();
			std::vector<_NL::Core::Object*> ActiveParticles;
			_NL::Core::Object* ParticleObj;
			//_NL::
			~ParticleSystem();
		};
	}
}

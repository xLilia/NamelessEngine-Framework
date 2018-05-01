#pragma once
#include "NamelessCore.h"
#include "MeshRenderer.h"
#include "GameObject.h"

#include <vector>

namespace _NL {
	namespace Object {
		class LightObject : public _NL::Core::Object
		{
		public:
			LightObject(std::string name);
			_NL::Core::LightProperties LightProperties;
			char* ClassName() const override;
		};
	}
}




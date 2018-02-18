#pragma once
#include "NamelessCore.h"
#include "MeshRenderer.h"
#include "GameObject.h"

#include <vector>

namespace _NL {
	namespace Object {
		class LightObject : public _NL::Object::GameObject
		{
		public:
			LightObject(std::string name);
			~LightObject();
			_NL::Core::LightProperties LightProperties;
			std::string ClassName() const override;
		};
	}
}




#pragma once
#include "NamelessCore.hpp"
#include "CppScript.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include <vector>
#include <typeinfo>

namespace _NL {
	namespace Object {
		class GameObject : public _NL::Core::Object
		{
		public:
			GameObject();
			GameObject(char* name);
			GameObject(const _NL::Core::Object &CO);
			//void getInfo() override; 
			char* getTypeName() override;

			///STATES
			///bool bactive = true;
			///bool bstatic = false;
		};
	}
}


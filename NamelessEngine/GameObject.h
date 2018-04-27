#pragma once
#include "NamelessCore.h"
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
			GameObject(std::string name);
			GameObject(const _NL::Core::Object &CO);
			~GameObject();
			void getInfo() override; 
			char* ClassName() const override;

			///STATES
			///bool bactive = true;
			///bool bstatic = false;
		};
	}
}


#pragma once
#include"NamelessCore.h"
#include "Script.h"
#include <vector>
#include <typeinfo>

namespace _NL {
	namespace Object {
		class GameObject : public _NL::Core::Object
		{
		public:
			GameObject();
			GameObject(std::string name);
			~GameObject();
			
			void getInfo() override; 
			std::string ClassName() const override;

			///STATES
			///bool bactive = true;
			///bool bstatic = false;
		};
	}
}


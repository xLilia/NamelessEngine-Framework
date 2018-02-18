#pragma once
#include"NamelessCore.h"
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

			int addComponent(_NL::Core::Component *C);

			template<typename T>
			T* getComponent(T ComponentType)
			{
				for each (_NL::Core::Component* c in this->Components)
				{
					if (c->ClassName() == ComponentType.ClassName()) {
						return (T*)c;
					}
				}
				return NULL;
			}
			///INFO
			GameObject *Parent; 
			void getInfo() override; 
			std::string ClassName() const override;
			///STATES
			///bool bactive = true;
			///bool bstatic = false;

		private:
			std::vector<_NL::Core::Component*> Components;
		};
	}
}


#pragma once
#include "NamelessCore.h"
namespace _NL {
	namespace Component {
		template<class T>
		class Script : public _NL::Core::Component
		{
		public:
			
			Script() {
				CreateNewScript(new T());
			}

			void CreateNewScript(T* ScriptClass) {
				cpps = ScriptClass;
			}

			T * _NL::Component::Script<T>::getScript(){
				return cpps;
			}

			std::string _NL::Component::Script<T>::ClassName() const override
			{
				return "_NL::Component::Script";
			}

		private:
			T* cpps;
		};

	}
}



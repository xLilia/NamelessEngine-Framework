#pragma once
#include "NamelessCore.h"
#include <fstream>

namespace _NL {
	namespace Component {
		template<typename T>
		class Script : public _NL::Core::Component
		{
		public:
			//Script();
			//~Script();
			
			void LinkScript(T* ScriptClass) {
				cpps = ScriptClass;
			}

			T * _NL::Component::Script<T>::getScript(){
				return cpps;
			}

			//std::string _NL::Component::Script<_NL::Core::Component>::ClassName() const override
			//{
			//	return "_NL::Component::Script";
			//}

		private:
			T* cpps;
		};
		
	}
}



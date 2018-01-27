#pragma once
#include "NamelessCore.h"

namespace _NL {
	namespace Component {
		class CppScript : _NL::Core::Component
		{
		public:
			CppScript(const char* FilePath);

			std::string ClassName() const override;
			~CppScript();
		};
	}
}



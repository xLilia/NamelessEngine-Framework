#pragma once
#include "NamelessCore.h"

namespace _NL {
	namespace Component {
		class Transform : public _NL::Core::Component
		{
		public:
			Transform();
			~Transform();

			_NL::Core::transform transform;

			///STATES
			bool bactive = true;
			///INFO
			std::string ClassName() const override;
			
		};
	}
}


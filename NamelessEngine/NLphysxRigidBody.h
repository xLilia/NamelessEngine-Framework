#pragma once
#include "NamelessCore.hpp"
#include "NLphysxCollider.h"

namespace _NL {
	namespace Component {
		class NLphysxRigidBody
		{
		public:
			NLphysxRigidBody();
			~NLphysxRigidBody();
			
			_NL::Element::NLphysxCollider* Collider;
		};

	}
}


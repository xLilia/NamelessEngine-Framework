#pragma once
#include "NamelessCore.h"
#include <glm\gtc\matrix_transform.hpp>

namespace _NL {
	namespace Component {
		class Transform : public _NL::Core::Component
		{
		public:
			Transform();
			~Transform();

			_NL::Core::transform transform;

			void Rotate(glm::vec3 EulerRotation);
			void Rotate(GLfloat x, GLfloat y, GLfloat z);

			///STATES
			bool bactive = true;
			///INFO
			std::string ClassName() const override;

			
			
		};
	}
}


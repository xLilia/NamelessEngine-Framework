#pragma once
#include "NamelessCore.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\euler_angles.hpp>

namespace _NL {
	namespace Component {
		class Transform : public _NL::Core::Component
		{
		public:
			Transform();
			~Transform();

			_NL::Core::transform transform;

			void EulerRotation(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0);
			void EulerRotation(glm::vec3 Axis);

			///STATES
			bool bactive = true;
			///INFO
			std::string ClassName() const override;
			
		};
	}
}


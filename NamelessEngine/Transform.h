#pragma once
#include "NamelessCore.hpp"
#include <glm\gtc\matrix_transform.hpp>

namespace _NL {
	namespace Component {
		class Transform : public _NL::Core::Component
		{
		public:
			Transform();
			~Transform();

			_NL::Core::transform transform;
			glm::vec3 eulerAngles();
			glm::quat LookAt(glm::vec3 target, glm::vec3 EyeAxis = glm::vec3(0,1,0));
			glm::quat RotateEuler(GLfloat x, GLfloat y, GLfloat z);
			glm::quat RotateEuler(glm::vec3 euleAngles);
			//glm::quat QuaternionFromAxisAngle(glm::vec3 ax, GLfloat angle);
			//void alignUpAxisWith(glm::vec3 up);
			///STATES
			bool bactive = true;
			///INFO
			char* ClassName() const override;
			
		};
	}
}


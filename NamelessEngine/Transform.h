#pragma once
#include "NamelessCore.hpp"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\matrix_decompose.hpp>

namespace _NL {
	namespace Component {
		class Transform : public _NL::Core::Component
		{
		public:
			Transform();
			~Transform();

			_NL::Core::transform transform;
			glm::vec3 getRotatedForwardVector();
			glm::vec3 getParentedVector(glm::vec3 Vector);
			glm::vec3 eulerAngles();
			glm::quat LookAt(glm::vec3 target, GLfloat x, GLfloat y, GLfloat z);
			glm::quat LookAt(glm::vec3 target, glm::vec3 EyeAxis = glm::vec3(0,1,0));
			glm::quat EulerRotation(GLfloat x, GLfloat y, GLfloat z);
			glm::quat EulerRotation(glm::vec3 euleAngles);
			glm::vec3 forwardVector = glm::vec3(0,0,-1);
			glm::mat4 ModelMatrix = glm::mat4(1.0f);
			void getModelMatrixDecompose(glm::vec3& Translation, glm::quat& Rotation, glm::vec3 scale);
			glm::vec3 getModelMatrixTranslation();
			glm::quat getModelMatrixRotation();
			glm::vec3 getModelMatrixScale();
			//glm::quat QuaternionFromAxisAngle(glm::vec3 ax, GLfloat angle);
			//void alignUpAxisWith(glm::vec3 up);
			///STATES
			bool bactive = true;
			///INFO
			char* getTypeName() override;
			
		};
	}
}


#pragma once
#include "NamelessCore.h"
#include <glm\gtc\matrix_transform.hpp>

namespace _NL {
	namespace Object {
		class CameraObj : public _NL::Core::Object
		{
		private:
			struct CamTransform
			{
				glm::vec3 position = glm::vec3(0, 0, -1);
				glm::vec3 rotation = glm::vec3(0, 1, 0);
				glm::vec3 LookAtCenter = glm::vec3(0, 0, 1);
			};

			struct CamSettings
			{
				GLfloat FOV;
				GLfloat NearPlane;
				GLfloat FarPlane;
				glm::vec2 RenderWindowPos;
				glm::vec2 RenderWindowSize;
				GLfloat RenderScaleRatio;
			};
		public:
			CameraObj(std::string name, GLsizei RenderWindowWidth, GLsizei RenderWindowHeight,
				GLsizei RenderWindowX = 0,
				GLsizei RenderWindowY = 0,
				GLfloat FOV = 90, 
				GLfloat NearPlane = 0.1f, 
				GLfloat FarPlane = 100,
				GLfloat RenderScaleRatio = 1);

			~CameraObj();
			void updateCameraSettings();
			void updateCameraProjectionMatrix();
			void updateCameraViewport();

			CamTransform Transform;
			CamSettings Settings;
			glm::mat4 getWorldToViewMatrix() const;
			glm::mat4 projectionMatrix;
			
			std::string ClassName() const override;
		};
	}
}



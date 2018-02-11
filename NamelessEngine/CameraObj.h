#pragma once
#include "NamelessCore.h"
#include <glm\gtc\matrix_transform.hpp>

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
	GLsizei RenderWindowX;
	GLsizei RenderWindowY;
	GLsizei RenderWindowWidth;
	GLsizei RenderWindowHeight;
};

namespace _NL {
	namespace Object {
		class CameraObj : public _NL::Core::Object
		{
		public:
			CameraObj(std::string name, GLsizei RenderWindowWidth, GLsizei RenderWindowHeight,
				GLsizei RenderWindowX = 0,
				GLsizei RenderWindowY = 0,
				GLfloat FOV = 90, 
				GLfloat NearPlane = .01f, 
				GLfloat FarPlane = 100);
			~CameraObj();
		
			CamTransform Transform;
			CamSettings Settings;
			glm::mat4 getWorldToViewMatrix() const;
			glm::mat4 projectionMatrix;
			void updateCameraSettings();//updateProjectionMatrix();
			std::string ClassName() const override;
		private:
			
		};
	}
}



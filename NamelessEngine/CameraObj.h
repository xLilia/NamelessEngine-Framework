#pragma once
#include "NamelessCore.h"
#include "WorldSpace.h"
#include <glm\gtc\matrix_transform.hpp>

namespace _NL {
	namespace Object {
		class CameraObj : _NL::Core::Object
		{
		public:
			CameraObj(std::string name, _NL::Object::WorldSpace* LinkedWorld, GLsizei RenderWindowWidth, GLsizei RenderWindowHeight,
				GLfloat FOV = 60, 
				GLfloat NearPlane = .01f, 
				GLfloat FarPlane = 100, 
				GLsizei RenderWindowX = 0, 
				GLsizei RenderWindowY = 0);
			~CameraObj();

			GLfloat FOV;
			GLfloat NearPlane;
			GLfloat FarPlane;
			GLsizei RenderWindowX;
			GLsizei RenderWindowY;
			GLsizei RenderWindowWidth;
			GLsizei RenderWindowHeight;	

			std::string name;
			_NL::Object::WorldSpace* LinkedWorld;

			void updateProjectionMatrix();

			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 LookAtCenter;

			glm::mat4 projectionMatrix;

			std::string ClassName() const override;
		private:
			
		};
	}
}



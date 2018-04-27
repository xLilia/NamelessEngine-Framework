#pragma once
#include "NamelessCore.h"
#include "ShaderInstance.h"
#include <SFML\Window\Window.hpp>
#include <SFML\Audio\Listener.hpp>
#include <glm\gtc\matrix_transform.hpp>

namespace _NL {
	namespace Object {
		class CameraObj : public _NL::Core::Object
		{
		public:
			CameraObj(std::string name, GLsizei RenderWindowWidth, GLsizei RenderWindowHeight,
				GLsizei RenderWindowX = 0,
				GLsizei RenderWindowY = 0,
				GLfloat FOV = 90.0f,
				GLfloat NearPlane = 0.1f,
				GLfloat FarPlane = 100,
				GLfloat RenderScaleRatio = 1,
				GLuint nMultisamples = 4,
				GLuint nRenderTextures = 2,
				bool HasPingPongShader = true,
				GLuint PingPongIterations = 10);
			~CameraObj();

			//---------------------------------------------------------------------------------
			//AUDIO
			//---------------------------------------------------------------------------------

			void updateAudioListenerWithCamTransform();
			void updateAudioListenerPosition(glm::vec3 pos);
			void updateAudioListenerDirection(glm::vec3 dir);
			void updateAudioListenerRotation(glm::vec3 upVec);

			sf::Listener AudioListener;

			//---------------------------------------------------------------------------------
			//FRAMEBUFFERS & RENDER
			//---------------------------------------------------------------------------------
			
			GLuint FinalQuadFrameBuffer;
			GLuint SceneRenderFrameBuffer;
			
			GLuint FinalQuad_ColorTexture;
			GLuint FinalQuad_DepthTexture;

			GLuint pingPongFBO[2];
			GLuint pingPongTexture[2];

			std::vector<GLuint> ColorTextures;
			GLuint DepthTexture;
			
			glm::vec3 ClearScreenColor = glm::vec3(0,0,0);

			glm::mat4 getWorldToViewMatrix() const;
			glm::mat4 getViewMatrix() const;
			glm::mat4 getProjectionMatrix() const;

			_NL::Element::ShaderInstance* PostProcessingShader;
			_NL::Element::ShaderInstance* PingPongShader;
			
			void ClearCurrentBuffer();

			void GenerateFrameBuffers();
			void PrepareToRenderScene();
			void DisplayOnScreen(GLuint* aditionalTextures = NULL);

			GLuint GeneratePingPongTexture();

			//---------------------------------------------------------------------------------
			//VARIABLES & UPDATES
			//---------------------------------------------------------------------------------

			//TRANSFORM
			glm::vec3 Position = glm::vec3(0, 0, 0);
			glm::vec3 UpAxis = glm::vec3(0, 1, 0);
			glm::vec3 LookAt = glm::vec3(0, 0, -1);

			//SETTINGS
			GLfloat FOV;
			GLfloat NearPlane;
			GLfloat FarPlane;
			glm::vec2 RenderWindowPos;
			glm::vec2 RenderWindowSize;
			GLfloat RenderScaleRatio;
			GLuint nMultisamples;
			GLuint nRenderTextures;
			bool HasPingPongShader;
			GLuint PingPongIterations;
			
			char* ClassName() const override;
					
			
		};
	}
}



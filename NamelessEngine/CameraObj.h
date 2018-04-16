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
				GLuint nMultisamples = 0,
				GLuint nColorTextures = 1,
				bool HasPingPongShader = false,
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
			
			GLuint PostProcessingFrameBuffer;
			GLuint SceneRenderFrameBuffer;
			
			GLuint PostProcessing_ColorTexture;
			GLuint PostProcessing_DepthTexture;

			GLuint pingPongFBO[2];
			GLuint pingPongTexture[2];

			std::vector<GLuint> ColorTextures;
			GLuint DepthTexture;
			//GLuint rboDepth;
			
			glm::vec3 ClearScreenColor = glm::vec3(1,0,1);

			glm::mat4 getWorldToViewMatrix() const;
			glm::mat4 getViewMatrix() const;
			glm::mat4 projectionMatrix;

			_NL::Element::ShaderInstance* PostProcessingShader;
			_NL::Element::ShaderInstance* PingPongShader;
			
			void GenerateFrameBuffers();
			void ClearCurrentBuffer();
			void PrepareToRenderScene();
			void DisplayOnScreen(sf::Window* W, GLuint* aditionalTextures = NULL);

			GLuint GeneratePingPongTexture();

			//---------------------------------------------------------------------------------
			//VARIABLES & UPDATES
			//---------------------------------------------------------------------------------

			//TRANSFORM
			glm::vec3 Position = glm::vec3(0, 0, 0);
			glm::vec3 Axis = glm::vec3(0, 1, 0);
			glm::vec3 LookAt = glm::vec3(0, 0, -1);

			//SETTINGS
			GLfloat FOV;
			GLfloat NearPlane;
			GLfloat FarPlane;
			glm::vec2 RenderWindowPos;
			glm::vec2 RenderWindowSize;
			GLfloat RenderScaleRatio;
			GLuint nMultisamples;
			GLuint nColorTextures;
			bool HasPingPongShader;
			GLuint PingPongIterations;

			void updateCameraSettings();
			void updateCameraProjectionMatrix();
			void updateCameraViewport();
			
			
			std::string ClassName() const override;
					
			
		};
	}
}



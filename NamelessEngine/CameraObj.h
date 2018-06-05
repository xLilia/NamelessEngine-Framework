#pragma once
#include "NamelessCore.hpp"
#include "ShaderInstance.h"
#include <SFML\Window\Window.hpp>
#include <SFML\Audio\Listener.hpp>
#include <glm\gtc\matrix_transform.hpp>

namespace _NL {
	namespace Object {
		class CameraObj : public _NL::Core::Object
		{
		public:
			CameraObj();
			CameraObj(std::string name, GLsizei RenderWindowWidth, GLsizei RenderWindowHeight,
				GLsizei RenderWindowX = 0,
				GLsizei RenderWindowY = 0,
				GLfloat FOV = 90.0f,
				GLfloat NearPlane = 0.1f,
				GLfloat FarPlane = 100,
				GLfloat RenderScaleRatio = 1,
				GLuint nRenderTextures = 8,
				bool nearestNeighbourFiltering = true
			);

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
			
			GLuint FinalQuad_FrameBuffer;
			GLuint G_FrameBuffer;
			
			
			//GLuint pingPongFBO[2];
			//GLuint pingPongTexture[2];

			std::vector<GLuint> ColorTextures;
			std::vector<GLuint> FinalQuad_ColorTextures;
			//GLuint FinalQuad_DepthTexture;
			//GLuint DepthTexture;

			GLuint DepthStencilTexture;
			GLuint StencilViewTexture;
			
			glm::vec3 ClearScreenColor = glm::vec3(0,0,0);

			glm::mat4 getWorldToViewMatrix() const;
			glm::mat4 getViewMatrix() const;
			glm::mat4 getProjectionMatrix() const;

			_NL::Element::ShaderInstance* FinalPassShader;
			//_NL::Element::ShaderInstance* PingPongShader;
			
			void ClearCurrentRenderTarget();
			void SetThisCamViewPort();
			void GenerateFrameBuffers();
			void SetCamAsRenderTarget();
			void DisplayOnScreen();

			//GLuint GeneratePingPongTexture();

			//---------------------------------------------------------------------------------
			//VARIABLES & UPDATES
			//---------------------------------------------------------------------------------

			//TRANSFORM
			struct transform {
				glm::vec3 Position = glm::vec3(0, 0, 0);
				glm::vec3 UpAxis = glm::vec3(0, 1, 0);
				glm::vec3 LookAt = glm::vec3(0, 0, -1);
			} transformCam;

			//SETTINGS
			GLfloat FOV;
			GLfloat NearPlane;
			GLfloat FarPlane;
			glm::vec2 RenderWindowPos;
			glm::vec2 RenderWindowSize;
			GLfloat RenderScaleRatio;
			GLuint nRenderTextures;
			bool nearestNeighbourFiltering;
			//GLuint nMultisamples;
			//bool HasPingPongShader;
			//GLuint PingPongIterations;
			
			char* getTypeName() const override;

		};
	}
}



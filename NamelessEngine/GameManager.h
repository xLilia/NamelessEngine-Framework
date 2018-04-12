#pragma once
#include "NL.hpp"
#include "GLError.h"

#include <SFML\Graphics.hpp>
#include <GL\glew.h>
#include <glm/gtc/type_ptr.hpp>

namespace _NL {
	namespace Engine {
		class GameManager
		{
		public:
			GameManager(const char * WindowName, int Width, int height, bool fullscreen = false, bool bVSync = true, int fpsLimit = 0);
			~GameManager();
		
			sf::Event Event;
			sf::RenderWindow* window;
			_NL::Engine::Time Time;
			GLfloat RenderExposure = 1.0;
			GLfloat RenderGamma = 1.0;
			glm::vec3 ClearColor;
			_NL::Engine::WorldSpace* CurrentScene;
			std::vector<_NL::Object::CameraObj*> Cameras;
			std::vector<_NL::Core::LightProperties> Lights;
			
			void RunCurrentScene();
			
			void EndCurrentScene();

		private:

			bool bEndCurrentScene = false;

			GLuint LightsBlockUBO;
			GLuint uIndexLightsBlock = 0;

			ShaderWizard ScreenShader;
			GLuint uRenderExposure = 1;
			GLuint uRenderGamma = 2;

			GLuint* FrameBuffer;
			GLuint* ColorTexture;
			GLuint* DepthTexture;
			
			//GLuint* ColorRenderBuffer;
			//GLuint* DepthRenderBuffer;

			void Start();
			void OpenGLStart();
			void GenerateCamFramebuffers(std::vector<_NL::Object::CameraObj*> Cams);
			void updateWindow();
			void UpdateCurrentScene();
			void DrawScreenQuad(GLuint CamID);
			void ClearCurrentBuffer();
			void RenderToScreen(GLuint CamID);
		
			void UpdateScriptsOfObj(_NL::Core::Object* obj);
			void StartScriptsOfObj(_NL::Core::Object* obj);

			void CleanUpLastSceneLoadedResources();
			///SwitchFrameBuffer
			//CurrentDrawBuff += 1;
			//CurrentDrawBuff %= 2;
		};
	}
}



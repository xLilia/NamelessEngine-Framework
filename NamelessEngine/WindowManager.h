#pragma once
#include "NL.hpp"
#include "GLError.h"

#include <SFML\Graphics.hpp>
#include <GL\glew.h>
#include <glm/gtc/type_ptr.hpp>


namespace _NL {
	namespace Engine {
		class WindowManager
		{
		public:
			WindowManager(const char * WindowName, int Width, int height, bool fullscreen = false, bool bVSync = true, int fpsLimit = 0, float renderScaleRatio = 1);
			void RunGameLoop();
			~WindowManager();
			sf::RenderWindow* window;
			_NL::Object::WorldSpace* CurrentScene;
			std::vector<_NL::Object::CameraObj*> Cameras;
			std::vector<_NL::Core::LightProperties> Lights;
		private:
			GLuint LightsBlockUBO;
			GLuint uIndexLightsBlock = 0;
			float renderScaleRatio;
			ShaderWizard ScreenShader;
			_NL::Engine::Time Time;
			GLuint FrameBuffer[1];
			GLuint ColorTexture;
			GLuint DepthTexture;
			GLuint ColorRenderBuffer[1];
			GLuint DepthRenderBuffer[1];
			GLuint CurrentDrawFrameBuffer = 0;
			void OpenGLStart();
			void Start();
			void DrawCurrentScene();
			void DrawScreenQuad();
			void ClearCurrentBuffer();
			void Display();
			void update();	

			///SwitchFrameBuffer
			//CurrentDrawBuff += 1;
			//CurrentDrawBuff %= 2;
		};
	}
}



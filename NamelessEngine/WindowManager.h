#pragma once
//#include"NamelessCore.h"
//#include "WorldSpace.h"
//#include"GameObject.h"
//#include"CameraObj.h"
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
			WindowManager(const char * WindowName, int Width, int height, bool fullscreen = false);
			void RunGameLoop();
			~WindowManager();
			sf::RenderWindow* window;
			_NL::Object::WorldSpace* CurrentScene;
			std::vector<_NL::Object::CameraObj*> Cameras;
		private:
			ShaderWizard ScreenShader;
		
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
			void Display();
			void update();	
		};
	}
}



#pragma once
//#include"NamelessCore.h"
//#include "WorldSpace.h"
//#include"GameObject.h"
//#include"CameraObj.h"
#include "NL.hpp"
#include "GLError.h"
#include <SFML\Graphics.hpp>
#include <GL\glew.h>

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
		private:
			void OpenGLStart();
			void Start();
			void DrawCurrentScene();
			void update();	
		};
	}
}



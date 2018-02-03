#pragma once
#include <SFML\Graphics.hpp>
#include <GL\glew.h>

namespace _NL {
	namespace Engine {
		class WindowManager
		{
		public:
			WindowManager(const char * WindowName, int Width, int height, bool fullscreen = false);
			void RunGameLoop();
			int update();
			~WindowManager();
			sf::RenderWindow* window;
		};
	}
}



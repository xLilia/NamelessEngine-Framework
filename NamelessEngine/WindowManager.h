#pragma once
#include <SFML\Graphics.hpp>

namespace _NL {
	namespace Engine {
		class WindowManager
		{
		public:
			WindowManager(const char * WindowName, int Width, int height, bool fullscreen = false);
			int update();
			~WindowManager();
			sf::RenderWindow* window;
		};
	}
}



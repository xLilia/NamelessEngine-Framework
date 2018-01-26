#pragma once
#include <SFML\Graphics.hpp>

class WindowManager
{
public:
	WindowManager(const char * WindowName, int Width, int height, bool fullscreen = false);
	int updateWindow();
	~WindowManager();
	sf::RenderWindow* window;
};


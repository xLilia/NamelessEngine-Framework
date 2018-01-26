#include "WindowManager.h"

int main(){

	WindowManager winMan("w1", 200, 200, false);

	while (winMan.window->isOpen())
	{
		winMan.updateWindow();
	}

	return 0;
}
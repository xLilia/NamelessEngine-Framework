# NAMELESS ENGINE - FRAMEWORK alpha0.6

Official Website & Doumentation: https://mattateusb7.github.io/NamelessEngine-Framework/

==================alpha0.6==========================

Out of the box included demo scene in MAIN.cpp

Demo scene Active Scripts controlls:

	1) Mouse movement - Look around
	
	2) left/right mouse buttons - increase/decrease FOV
	
	3) WASD QE - Move camera
	
	4) TFGH RY - Rotate objs
	
	5) U - Reset Objects
	
	6) J - Objs follow camera
	
	7) ESC - End scene

Current Problems and what to do:
	
	1) GL_INVALID_OPERATION - cameraobj.cpp:147 ||| Ignore.
	
	2) Low framerate and or stream of GL_Errors in the console ||| Restart Program.
	
	3) Skybox disappearance ||| Restart Program.
	
	4) Weird Textures ||| Restart Program.
	
Current bugged out Features:

	1) PARTICLE SYSTEMS not rendering particles ||| does not work, may cause crash.
	
	2) UItext ||| does not work.
	
	3) Instantiation of objects in scene during game loop ||| undefined behaviour, may cause crash.
	
Currently Supported 3D Model Files: 

	1) ".obj"(OBJ files) and ".dae"(COLLADA files)

==================alpha0.6==========================

Game Engine SDK

Developer: Mateus Branco

Platform: PC-Windows

Written in: C++

Minimum System Requirements: 

●	OpenGL v4.5

Available in: English

Included External Libraries: 

●	Simple and Fast Multimedia Library  (SFML) v2.1.0

○	https://www.sfml-dev.org/

●	The OpenGL Extension Wrangler Library (GLEW) v2.4.2

○	http://glew.sourceforge.net/

●	OpenGL Mathematics (GLM) v0.9.8.0

○	https://glm.g-truc.net

●	NVIDIA GameWorks PhysX-3.4 SDK [not yet implemented]

○	https://developer.nvidia.com/physx-sdk

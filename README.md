# NAMELESS ENGINE - FRAMEWORK alpha0.6

Official Website & Doumentation: https://mattateusb7.github.io/NamelessEngine-Framework/

======================= Current Version Notes: alpha0.9 =========================

Out of the box included demo scene in MAIN.cpp

Tech-Demo Scripts controlls:

	-F1- CockpitCamera
	-Number 0: FreeCamera
	-Number 9-7: change skybox
	-Number 6: disable skybox
	-Rshift: exposure Up
	-RControl: exposure down
	-Rshift + Dash: gamma Up
	-RControl+ Dash: gamma down
	
	-when on CockpitCamera
	-F3- Top camera
	-F4- BottomCamera
	-WS - Forward / Backward
	-AD - Turn Left / Turn Right
	-QE - Strife Left / Strife Right
	-LShift LControll - Up / Down
	-Spacebar : Breaks
	
	-when on Top or bottom camera:
	-F5 - Normal Vision
	-F6 - NightVision
	-when on Night Vision:
	-LeftRight Mouse Button : Zoom IN/OUT
	
	-when on FreeCamera
	-Number 1: no light
	-Number 2: white pointLight
	-Number 3: multicolor pointLight
	-Number 4: White spotLight
	-WASDQE-Movement
	-LControl- SpeedUp

Current Problems and what to do:
	
	1) GL_INVALID_OPERATION - cameraobj.cpp:147 ||| Ignore.
	
	2) Low framerate and or stream of GL_Errors in the console ||| Restart Program.
	
	3) Skybox disappearance ||| Restart Program.
	
	4) Weird & or rotated Textures ||| Restart Program.
	
Current bugged out Features:

	1) PARTICLE SYSTEMS not rendering particles ||| does not work, may cause crash.
	
	2) UItext ||| does not display textures correctly.
	
	3) Obj Instantiation funnctions ||| undefined behaviour, may cause crash.
	
======================= Current Version Notes: alpha0.9 =========================

======================= Features ==========================

Implemented:

	- [x] Custom Multi-Resolution.
	- [x] Window/Borderless/Fullscreen Modes
	- [x] Down sampling Rendering.
	- [x] Linear or Nearest Image Filtering
	- [x] Deffered Rendering.
	- [x] Defered Lighting.
	- [x] Point, Directional and spotlight Light types.
	- [x] Custom PBR Materials.
	- [x] Multiple Viewport Rendering.
	- [x] Postprocessing stack for each camera.
	- [x] Custom PostProcessing Effects.
	- [x] Skybox Generation from cubemap (6 images) or from a single Image.
	- [x] HDRI Images for Skybox.
	- [x] Precomputed Environmental Lighting from skybox.
	- [x] Reflexion of skybox in polished materials.
	- [x] Seamless transition between Multiple scenes.
	- [/] Object Instantiation and destruction.
	- [x] Component based Architeture.
	- [x] Scriptable Objects.
	- [x] Texture Loader.
	- [x] OBJ and COLLADA Model Loader.
	- [x] UI Canvas.
	- [x] UI Images.
	- [/] UI Text.
	- [/] Custom Shaders.
	- [ ] Nvidia PhysX Implementation.
	- [ ] Shadow Casting.
	- [/] Model Animator.
	- [/] Particle Systems with Scriptable Particles.
	- [/] 3D Audio
	
	...
======================= Features ==========================

======================= DEV LOG ==========================

Currently Supported 3D Model Files: 

	1) ".obj"(OBJ files) and ".dae"(COLLADA files).
	
Planned features:

	1) Shadow casting.
	
	2) UIButtons & UIText.
	
	3) CPU Particle Systems with programable particles.
	
	4) Better 3D Audio.
	
	5) Nvidia PhysX.
	
	6) Animations.

======================= DEV LOG ==========================

Game Engine SDK

Developer: Mateus Branco

Platform: PC-Windows

Written in: C++

Minimum System Requirements: 

- OpenGL v4.5

Available in: English

Included External Libraries: 

- Simple and Fast Multimedia Library  (SFML) v2.1.0
  
- https://www.sfml-dev.org/
  
- The OpenGL Extension Wrangler Library (GLEW) v2.4.2
  
- http://glew.sourceforge.net/
  
- OpenGL Mathematics (GLM) v0.9.8.0
  
- https://glm.g-truc.net
  
- NVIDIA GameWorks PhysX-3.4 SDK [not yet implemented]
  
- https://developer.nvidia.com/physx-sdk

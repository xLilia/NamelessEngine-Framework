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
			sf::Window* window;
			_NL::Engine::Time GameTime;
			_NL::Engine::WorldSpace* CurrentScene;
			std::vector<_NL::Object::CameraObj*> Cameras;
			std::vector<_NL::Core::LightProperties> Lights;
			std::vector<_NL::Core::UI*> UICanvas;
			glm::vec3 ClearScreenColor = glm::vec3();

			void RunCurrentScene();
			void EndCurrentScene();

			void Start();
			void OpenGLStart();

			void GameTick();

			void RenderCurrentScene();

			void UpdateSceneLights();
			void RenderSceneSkybox(_NL::Object::CameraObj* Cam);
			void RenderSceneObjects(_NL::Object::CameraObj* Cam);
			void RenderScreenQuad(_NL::Object::CameraObj* Cam);
			void RenderSceneCanvas();

			void UpdateScriptsOfObj(_NL::Core::Object* obj);
			void StartScriptsOfObj(_NL::Core::Object* obj);

			void CleanUpLastSceneLoadedResources();

		private:

			bool bEndCurrentScene = false;

			GLuint LightsBlockUBO;
			GLuint uIndexLightsBlock = 0;

			ShaderWizard ScreenShader;
			GLuint uRenderExposure = 1;
			GLuint uRenderGamma = 2;

		};
	}
}



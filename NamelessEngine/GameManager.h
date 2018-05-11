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
			GameManager(const char * WindowName, int Width, int height, bool fullscreen = false, bool bVSync = true, int fpsLimit = 60);
			~GameManager();
		
			sf::Event Event;
			sf::Window* window;
			_NL::Engine::Time GameTime;
			_NL::Engine::WorldSpace* CurrentScene;
			_NL::Element::ShaderInstance* DepthPassShader;
			
			std::vector<GLuint> ShadowMaps;
			GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

			std::vector<_NL::Object::CameraObj*> Cameras;
			std::vector<_NL::Core::LightProperties> LightsProperties; //MAKE REFERENCE
			std::vector<_NL::Object::LightObject*>Lights;

			std::vector<_NL::Object::ParticleSystem*> ParticleSystems;
			std::vector<_NL::Core::UI*> UICanvas;

			glm::vec3 ClearScreenColor = glm::vec3();

			void RunScene(_NL::Engine::WorldSpace* set_current_scene);
			void EndCurrentScene(_NL::Engine::WorldSpace* load_next_scene = nullptr);
			void OpenGLStart();

			void GameTick();

			void RenderCurrentScene();
			void UpdateSceneLights();
			void UpdateParticleSystems();
			void RenderSceneSkybox(glm::mat4 WorldToViewMatrix, glm::mat4 ProjectionMatrix);
			void RenderSceneObjects(glm::vec3 EyePos, glm::mat4 WorldToViewMatrix, glm::mat4 ProjectionMatrix, GLuint UseAlternativeShaderProgram = NULL);
			void RenderScreenQuad(_NL::Object::CameraObj* Cam);
			void RenderSceneCanvas();

			void StartUpdateScriptsOfObj(_NL::Core::Object* obj);
			void EndScriptsOfObj(_NL::Core::Object * obj);

			void CleanUpCurrentSceneLoadedResources();

		private:

			bool bEndCurrentScene = false;

			GLuint LightsBlockUBO;
			GLuint uIndexLightsBlock = 0;

		};
	}
}



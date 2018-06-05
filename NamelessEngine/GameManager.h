#pragma once
#include "NamelessCore.hpp"
#include "NL_Time.h"
#include "WorldSpace.h"
#include "LightObject.h"
#include "ParticleSystem.h"
#include "UICanvas.h"
#include "GLError.h"
#include <SFML\Graphics.hpp>
#include <GL\glew.h>
#include <glm/gtc/type_ptr.hpp>

namespace _NL {
	namespace Engine {
		class GameManager
		{
		public:
			GameManager(std::string WindowName, int Width, int Height, bool hideMouse=false, bool fullscreen = false, bool borderless = false, bool bVSync = false, int fpsLimit = 0);
			~GameManager();
			
			sf::Event Event;
			sf::RenderWindow* window;
			std::string WindowName;
			_NL::Engine::Time GameTime;
			_NL::Engine::WorldSpace* CurrentScene;
			std::vector<_NL::Object::CameraObj*> Cameras;
			std::vector<_NL::Object::LightObject*>Lights;
			std::vector<_NL::Object::ParticleSystem*> ParticleSystems;
			std::vector<_NL::UI::UICanvas*> UICanvas;

			void RunScene(_NL::Engine::WorldSpace* set_current_scene);
			void EndCurrentScene(_NL::Engine::WorldSpace* load_next_scene = nullptr);
			void GoFullscreen();
			void GoWindowed(GLfloat Width, GLfloat Height);
			void GoBorderless(GLfloat Width, GLfloat Height);
			void GameTick();

			void UpdateObjectLists();
			void SafeKillObj(_NL::Core::Object* Target);

			template<class CastToObjType>
			CastToObjType* SafeInstantiateObj(_NL::Core::Object * Target)
			{
				CastToObjType* Inst;
				if (Target != nullptr) {
					Inst = CurrentScene->Instantiate<CastToObjType>(Target);
				}
				return Inst;
			}

			void RenderCurrentScene();
			void UpdateSceneLights();
			void UpdateParticleSystems();
			void RenderSceneSkybox(_NL::Object::CameraObj* Cam);
			void RenderSceneObjects(_NL::Object::CameraObj* Cam, GLuint UseOverrideShaderProgram = NULL);
			void RenderScreenQuad(_NL::Object::CameraObj* Cam);
			void RenderSceneCanvas();

			void StartUpdateScriptsOfObj(_NL::Core::Object* obj);
			void EndScriptsOfObj(_NL::Core::Object * obj);

			void CleanUpCurrentSceneLoadedResources();
	
		private:

			bool bEndCurrentScene = false;

			GLuint LightsSSBO;
			GLuint LightsSSBOBindingPoint = 0;

		};
	}
}



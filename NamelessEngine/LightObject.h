#pragma once
#include "NamelessCore.hpp"
#include "GameManager.h"
#include <vector>

namespace _NL {
	namespace Object {
		class LightObject : public _NL::Core::Object
		{
		public:
			LightObject(std::string name);
			void GenerateFramebuffer();
			void UpdateFramebuffer(_NL::Engine::GameManager* G);
			_NL::Core::LightProperties LightProperties;
			GLuint Framebuffer;
			GLuint ShadowMap;
			const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
			char* ClassName() const override;
		};
	}
}




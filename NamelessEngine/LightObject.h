#pragma once
#include "NamelessCore.hpp"
#include <vector>

namespace _NL {
	namespace Object {
		class LightObject : public _NL::Core::Object
		{
		public:
			LightObject(std::string name);
			void GenerateFramebuffer(GLuint Shadow_Width, GLuint Shadow_Height);
			_NL::Core::LightProperties LightProperties;
			
			GLuint ShadowMap;
			GLuint Framebuffer;
			bool bCastsShadows = true;

			char* ClassName() const override;
		};
	}
}




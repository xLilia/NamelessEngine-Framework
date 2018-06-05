#pragma once
#include "NamelessCore.hpp"
#include <vector>

namespace _NL {
	namespace Object {
		class LightObject : public _NL::Core::Object
		{
		public:
			LightObject();
			LightObject(std::string name);
			void GenerateFramebuffer(GLuint Shadow_Width, GLuint Shadow_Height);
			_NL::Core::LightProperties LightProperties;
			
			GLuint ShadowMap;
			GLuint Framebuffer;
			bool bCastsShadows = true;

			char* getTypeName() const override;
		};
	}
}




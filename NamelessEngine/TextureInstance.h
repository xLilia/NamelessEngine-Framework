#pragma once
#include "NamelessCore.hpp"
#include "TextureLoader.h"
namespace _NL {
	namespace Element {
		class TextureInstance
		{
		public:
			TextureInstance(const char* filepath, bool Nearest, bool FlipHorizontaly = false, bool FlipVertically = true);
			GLuint getTextureID();
			glm::vec2 getWidthHeight();
			~TextureInstance();
		private:
			GLuint TextureID;
			glm::vec2 widthHeight;
			_NL::Tools::TextureLoader TL;
		};


	}
}

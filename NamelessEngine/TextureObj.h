#pragma once
#include"NamelessCore.h"
#include <SFML\Graphics\Image.hpp>

namespace _NL {
	namespace Element {
		class TextureObj : public _NL::Core::Element
		{
		public:
			TextureObj(const std::string filePath);
			
			~TextureObj();
		private:
			sf::Image Texture;
		};
	}
}

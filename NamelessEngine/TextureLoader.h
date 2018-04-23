#pragma once
#include"NamelessCore.h"
#include <vector>
#include <SFML\Graphics\Image.hpp>
#include <SFML\Graphics\Text.hpp>

namespace _NL {
	namespace Tools {
		class TextureLoader
		{
		public:
			TextureLoader();
			~TextureLoader();
			int LoadImage(const char* filePath);
			GLuint GenerateTexure(const char* filePath, bool Nearest = false, bool FlipHorizontaly = false, bool FlipVertically = true);
			void RESET();
			sf::Image LoadedTexture;
			std::vector<GLuint> GLTexIDs;
			sf::Text T;
		};
	}
}


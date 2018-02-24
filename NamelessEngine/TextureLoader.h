#pragma once
#include"NamelessCore.h"
#include <vector>
#include <SFML\Graphics\Image.hpp>


class TextureLoader
{
public:
	TextureLoader();
	~TextureLoader();
	int LoadImage(const char* filePath);
	int GenerateTexure(const char* filePath = 0);
	void RESET();
	sf::Image LoadedImage;
	std::vector<GLuint> GLTexIDs;
private:
	
};

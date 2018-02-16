#pragma once
#include"NamelessCore.h"
#include <vector>
#include <SFML\Graphics\Image.hpp>


class TextureLoader
{
public:
	TextureLoader();
	~TextureLoader();
	int LoadTexure(const std::string filePath);
	void RESET();
	std::vector<GLuint> GLTexIDs;
private:
	
};

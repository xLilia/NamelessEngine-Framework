#include "TextureLoader.h"

TextureLoader::TextureLoader()
{
	
}

int TextureLoader::LoadTexure(const std::string filePath)
{
	sf::Image TextureImage;
	if (!TextureImage.loadFromFile(filePath)) {
		return -1;
	}
	GLuint GLTexID;
	glCreateTextures(GL_TEXTURE_2D, 1, &GLTexID);
	glBindTexture(GL_TEXTURE_2D, GLTexID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		TextureImage.getSize().x,
		TextureImage.getSize().y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		TextureImage.getPixelsPtr());
	glGenerateMipmap(GLTexID);
	GLTexIDs.push_back(GLTexID);
}

void TextureLoader::RESET()
{
	GLTexIDs.clear();
}

TextureLoader::~TextureLoader() {
	//glDeleteTextures(GLTexIDs.size(), &GLTexIDs[0]);
}
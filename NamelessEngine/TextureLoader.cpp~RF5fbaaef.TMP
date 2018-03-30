#include "TextureLoader.h"

_NL::Tools::TextureLoader::TextureLoader()
{
	
}

int _NL::Tools::TextureLoader::GenerateTexure(const char* filePath)
{

	if (filePath != 0) {
		LoadImage(filePath);
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
		LoadedImage.getSize().x,
		LoadedImage.getSize().y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		LoadedImage.getPixelsPtr());
	glGenerateMipmap(GLTexID);
	GLTexIDs.push_back(GLTexID);

	return 0;
}

int _NL::Tools::TextureLoader::LoadImage(const char* filePath)
{
	
	if (!LoadedImage.loadFromFile(filePath)) {
		return -1;
		std::cout << "ERROR: FAILED TO LOAD: " << filePath << std::endl;
	}
	return 0;
}


void _NL::Tools::TextureLoader::RESET()
{
	GLTexIDs.clear();
}

_NL::Tools::TextureLoader::~TextureLoader() {
	//glDeleteTextures(GLTexIDs.size(), &GLTexIDs[0]);
}


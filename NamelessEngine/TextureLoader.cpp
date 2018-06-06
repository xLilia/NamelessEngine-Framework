#include "TextureLoader.h"

_NL::Tools::TextureLoader::TextureLoader()
{
	
}

GLuint _NL::Tools::TextureLoader::GenerateTexure(const char* filePath, bool Nearest, bool FlipHorizontaly, bool FlipVertically)
{

	if (filePath != 0) {
		LoadImage(filePath);
	}

	if (FlipHorizontaly) {
		LoadedTexture.flipHorizontally();
	}

	if (FlipVertically) {
		LoadedTexture.flipVertically();
	}
	
	GLuint GLTexID;
	glCreateTextures(GL_TEXTURE_2D, 1, &GLTexID);
	glBindTexture(GL_TEXTURE_2D, GLTexID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if (Nearest) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		LoadedTexture.getSize().x,
		LoadedTexture.getSize().y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		LoadedTexture.getPixelsPtr()
	);
	GLTexIDs.push_back(GLTexID);
	return GLTexID;
}

int _NL::Tools::TextureLoader::LoadImage(const char* filePath)
{
	if (!LoadedTexture.loadFromFile(filePath)) {
		return -1;
		std::cout << "ERROR: FAILED TO LOAD: " << filePath << std::endl;
	}
	return 0;
}


void _NL::Tools::TextureLoader::RESET()
{
	GLTexIDs.clear();
	GLTexIDs.shrink_to_fit();
}

_NL::Tools::TextureLoader::~TextureLoader() {
	//glDeleteTextures(GLTexIDs.size(), &GLTexIDs[0]);
}


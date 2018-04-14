#include "TextureLoader.h"

_NL::Tools::TextureLoader::TextureLoader()
{
	
}

GLuint _NL::Tools::TextureLoader::GenerateTexure(const char* filePath, bool bSaveTexID)
{

	if (filePath != 0) {
		LoadImage(filePath);
	}
	
	GLuint GLTexID;
	glCreateTextures(GL_TEXTURE_2D, 1, &GLTexID);
	glBindTexture(GL_TEXTURE_2D, GLTexID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
	//glGenerateMipmap(GLTexID);
	if(bSaveTexID)
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
}

_NL::Tools::TextureLoader::~TextureLoader() {
	//glDeleteTextures(GLTexIDs.size(), &GLTexIDs[0]);
}


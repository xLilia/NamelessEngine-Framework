#include "TextureInstance.h"


_NL::Element::TextureInstance::TextureInstance(const char * filepath, bool Nearest, bool FlipHorizontaly, bool FlipVertically)
{
	TextureID = TL.GenerateTexure(filepath, Nearest, FlipHorizontaly, FlipVertically);
	this->widthHeight = glm::vec2(TL.LoadedTexture.getSize().x, TL.LoadedTexture.getSize().y);
}

GLuint _NL::Element::TextureInstance::getTextureID()
{
	return this->TextureID;
}

glm::vec2 _NL::Element::TextureInstance::getWidthHeight()
{
	return this->widthHeight;
}

_NL::Element::TextureInstance::~TextureInstance()
{
}

#include "UIImage.h"

_NL::UI::UIImage::UIImage()
{

}

_NL::UI::UIImage::UIImage(char* const filepath)
{
	LoadTexture(filepath);
}


_NL::UI::UIImage::~UIImage()
{
}

void _NL::UI::UIImage::LoadTexture(char * const filepath)
{
	TextureID = TL.GenerateTexure(filepath);
	widthHight = glm::vec2(TL.LoadedTexture.getSize().x, TL.LoadedTexture.getSize().y);
}

std::string _NL::UI::UIImage::ClassName() const
{
	return "_NL::UI::UIImage";
}

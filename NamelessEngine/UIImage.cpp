#include "UIImage.h"

_NL::UI::UIImage::UIImage(_NL::Element::TextureInstance* Texture)
{
	this->Texture = Texture;
	widthHeight = Texture->getWidthHeight();
}


_NL::UI::UIImage::~UIImage()
{
}
	

std::string _NL::UI::UIImage::ClassName() const
{
	return "_NL::UI::UIImage";
}

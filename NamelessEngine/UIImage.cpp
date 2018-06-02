#include "UIImage.h"

_NL::UI::UIImage::UIImage(_NL::Element::TextureInstance* Texture)
{
	this->Texture = Texture;
	widthHeight = Texture->getWidthHeight();
}


_NL::UI::UIImage::~UIImage()
{
}

void _NL::UI::UIImage::DrawImage(_NL::Object::CameraObj* TargetCam)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUniform1i(0, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, Texture->getTextureID());
	glm::vec2 finalPos = (AnchorPosition + PositionRelativeToAnchor) + AnchorPosition;
	finalPos.x = TargetCam->RenderWindowSize.x - finalPos.y;
	finalPos.y = TargetCam->RenderWindowSize.y - finalPos.y;
	glm::vec2 finalScale = (widthHeight * scale);
	finalScale.x = widthHeight.x; //* (widthHeight.x / TargetCam->RenderWindowSize.x);
	finalScale.y = widthHeight.y; //* (widthHeight .y / TargetCam->RenderWindowSize.y);
	finalPos.y - finalScale.y;
	_NL::Core::RenderQuad(finalPos.x, finalPos.y, finalScale.x, finalScale.y, false);
	glDisable(GL_BLEND);
}
	

char* _NL::UI::UIImage::ClassName() const
{
	return "_NL::UI::UIImage";
}

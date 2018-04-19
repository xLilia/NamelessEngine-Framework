#include "UICanvas.h"



_NL::UI::UICanvas::UICanvas()
{
}


_NL::UI::UICanvas::~UICanvas()
{
}

void _NL::UI::UICanvas::addUIElement(_NL::Core::UI* UI)
{
	UIelements.push_back(UI);
}

void _NL::UI::UICanvas::DrawElements()
{
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	ImageRenderShader->Use();
	for each (_NL::Core::UI* UI in UIelements)
	{
		if (UI->ClassName() == "_NL::UI::UIImage") {

			_NL::UI::UIImage* UII = dynamic_cast<_NL::UI::UIImage*>(UI);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, UII->TextureID);
			glm::vec2 finalPos = (UI->AnchorPosition + UI->PositionRelativeToAnchor) + AnchorPosition;
			glm::vec2 finalScale = (UII->widthHight * UII->scale);
			_NL::Core::RenderQuad(finalPos.x, finalPos.y, finalScale.x, finalScale.y);
		
		}
	}
	ImageRenderShader->UnUse();
	glEnable(GL_DEPTH_TEST);
}

std::string _NL::UI::UICanvas::ClassName() const
{
	return "_NL::UI::UICanvas";
}

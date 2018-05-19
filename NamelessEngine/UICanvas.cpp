#include "UICanvas.h"


_NL::UI::UICanvas::UICanvas(sf::RenderWindow* GameManagerWindow)
{
	this->WindowTarget = GameManagerWindow;
}


bool sortFunc(_NL::Core::UI* a, _NL::Core::UI* b) { return (a->Layer < b->Layer); };

void _NL::UI::UICanvas::addUIElement(_NL::Core::UI* UI)
{
	UIelements.push_back(UI);
	std::sort(UIelements.begin(), UIelements.end(), sortFunc);
}

void _NL::UI::UICanvas::DrawElements()
{
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	ImageRenderShader->UnUse();
	for each (_NL::Core::UI* UI in UIelements)
	{
		if (UI->ClassName() == "_NL::UI::UICanvas") {
			_NL::UI::UICanvas* UII = dynamic_cast<_NL::UI::UICanvas*>(UI);
			UII->DrawElements();
		}

		if (UI->ClassName() == "_NL::UI::UIImage") {
			
			_NL::UI::UIImage* UII = dynamic_cast<_NL::UI::UIImage*>(UI);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, UII->Texture->getTextureID());
			glm::vec2 finalPos = (UI->AnchorPosition + UI->PositionRelativeToAnchor) + AnchorPosition;
			glm::vec2 finalScale = (UII->widthHeight * UII->scale);
			_NL::Core::RenderQuad(finalPos.x, WindowTarget->getSize().y - finalPos.y - finalScale.y, finalScale.x, finalScale.y, false);
		
		}

		if (UI->ClassName() == "_NL::UI::UIText") {
			_NL::UI::UIText* UII = dynamic_cast<_NL::UI::UIText*>(UI);
			UII->DrawText(this->WindowTarget);
		}
		
	}
	ImageRenderShader->UnUse();
	glEnable(GL_DEPTH_TEST);
}

char* _NL::UI::UICanvas::ClassName() const
{
	return "_NL::UI::UICanvas";
}

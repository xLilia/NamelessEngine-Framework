#include "UICanvas.h"


_NL::UI::UICanvas::UICanvas()
{

}

_NL::UI::UICanvas::UICanvas(sf::RenderWindow* NLManagerWindow)
{
	this->WindowTarget = NLManagerWindow;
}


bool sortFunc(_NL::Core::UI* a, _NL::Core::UI* b) { return (a->Layer < b->Layer); };

void _NL::UI::UICanvas::addUIElement(_NL::Core::UI* UI)
{
	UIelements.push_back(UI);
	std::sort(UIelements.begin(), UIelements.end(), sortFunc);
}

void _NL::UI::UICanvas::DrawElements()
{

	for (_NL::Core::UI* UI : UIelements)
	{
		if (UI->getTypeName() == "_NL::UI::UICanvas") {
			_NL::UI::UICanvas* UII = dynamic_cast<_NL::UI::UICanvas*>(UI);
			UII->DrawElements();
		}

		if (UI->getTypeName() == "_NL::UI::UIImage") {
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			ImageRenderShader->Use();
			_NL::UI::UIImage* UII = dynamic_cast<_NL::UI::UIImage*>(UI);
			glUniform1i(1, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, UII->Texture->getTextureID());
			glm::vec2 finalPos = (UI->AnchorPosition + UI->PositionRelativeToAnchor) + AnchorPosition;
			glm::vec2 finalScale = (UII->widthHeight * UII->scale);
			_NL::Core::RenderQuad(finalPos.x, WindowTarget->getSize().y - finalPos.y - finalScale.y, finalScale.x, finalScale.y, false);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
			ImageRenderShader->UnUse();
			glDisable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
		}    

		if (UI->getTypeName() == "_NL::UI::UIText") {
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			
			glViewport(0, 0, WindowTarget->getSize().x, WindowTarget->getSize().y);
			_NL::UI::UIText* UII = dynamic_cast<_NL::UI::UIText*>(UI);
			UII->DrawText(this->WindowTarget);
			glDisable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
		}
		
	}

}

char* _NL::UI::UICanvas::getTypeName() const
{
	return "_NL::UI::UICanvas";
}

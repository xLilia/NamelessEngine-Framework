#include "UIText.h"



_NL::UI::UIText::UIText(const char * file_Path, std::string S)
{
	setFont(file_Path);
	setString(S);	
}

_NL::UI::UIText::~UIText()
{
}

void _NL::UI::UIText::setString(std::string S)
{
	T.setString(S);
}

std::string _NL::UI::UIText::getString()
{
	return T.getString();
}

void _NL::UI::UIText::setFont(const char * file_Path)
{
	F.loadFromFile(file_Path);
	T.setFont(F);
}

void _NL::UI::UIText::DrawText(sf::RenderWindow* WindowTarget)
{
	T.setCharacterSize(TextProperties.CharacterSize);
	T.setFillColor(sf::Color(TextProperties.FillColor.r, TextProperties.FillColor.g, TextProperties.FillColor.b, TextProperties.FillColor.a));
	T.setOutlineColor(sf::Color(TextProperties.OutlineColor.r, TextProperties.OutlineColor.g, TextProperties.OutlineColor.b, TextProperties.OutlineColor.a));
	T.setOutlineThickness(TextProperties.OutlineThickness);
	T.setStyle(TextProperties.TextStyle);

	glm::vec2 finalPos = (this->AnchorPosition + this->PositionRelativeToAnchor);
	T.setPosition(sf::Vector2f(finalPos.x, finalPos.y));
	WindowTarget->draw(T);
}

char * _NL::UI::UIText::ClassName() const
{
	return "_NL::UI::UIText";
}

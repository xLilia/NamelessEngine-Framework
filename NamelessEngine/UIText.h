#pragma once
#include "NamelessCore.hpp"
#include <SFML\Graphics.hpp>
namespace _NL {
	namespace UI {
		class UIText : public _NL::Core::UI
		{
		public:
			UIText(const char * font_file_path, std::string text);

			enum Style
			{
				Regular = sf::Text::Regular,
				Bold = sf::Text::Bold,
				Italic = sf::Text::Italic,
				StrikeThrough = sf::Text::StrikeThrough,
				Underlined = sf::Text::Underlined
			};
			
			struct TextProperties {
				Style TextStyle = Style::Regular;
				GLuint CharacterSize = 30;
				glm::vec4 FillColor = glm::vec4(0, 0, 255, 255);
				glm::vec4 OutlineColor = glm::vec4(0, 0, 0, 255);
				GLfloat OutlineThickness = 1.0;
			}TextProperties;

			~UIText();
			void setString(std::string S);
			std::string getString();
			void setFont(const char* file_Path);
			void DrawText(sf::RenderWindow* WindowTarget);

			virtual char* getTypeName() const override;
			sf::Text T;
			sf::Font F;
		private:
			
		};
	}
}

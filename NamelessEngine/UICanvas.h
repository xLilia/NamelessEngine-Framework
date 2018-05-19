#pragma once
#include "UIImage.h"
#include "UIText.h"
#include "ShaderInstance.h"
#include <SFML\Graphics.hpp>

namespace _NL {
	namespace UI {
		class UICanvas : public _NL::Core::UI, public _NL::Core::Object
		{
		public:
			UICanvas(sf::RenderWindow* GameManagerWindow);

			sf::RenderWindow* WindowTarget;
			_NL::Element::ShaderInstance* ImageRenderShader;

			std::vector<_NL::Core::UI*> UIelements;

			void addUIElement(_NL::Core::UI* UI);
			void DrawElements();

			char* ClassName() const override;
		};
	}
}

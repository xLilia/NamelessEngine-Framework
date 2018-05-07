#pragma once
#include "NamelessCore.hpp"
#include "UIImage.h"
#include "ShaderInstance.h"

namespace _NL {
	namespace UI {
		class UICanvas : public _NL::Core::UI, public _NL::Core::Object
		{
		public:
			UICanvas();

			_NL::Element::ShaderInstance* ImageRenderShader;

			std::vector<_NL::Core::UI*> UIelements;

			void addUIElement(_NL::Core::UI* UI);
			void DrawElements(GLfloat ScreenHeight);

			char* ClassName() const override;
		};
	}
}

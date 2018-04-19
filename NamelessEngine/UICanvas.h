#pragma once
#include "NamelessCore.h"
#include "UIImage.h"
#include "ShaderInstance.h"

namespace _NL {
	namespace UI {
		class UICanvas : public _NL::Core::UI, public _NL::Core::Object
		{
		public:
			UICanvas();
			~UICanvas();

			_NL::Element::ShaderInstance* ImageRenderShader;

			std::vector<_NL::Core::UI*> UIelements;

			void addUIElement(_NL::Core::UI* UI);
			void DrawElements();

			std::string ClassName() const override;
		};
	}
}

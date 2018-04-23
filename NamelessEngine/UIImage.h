#pragma once
#include "NamelessCore.h"
#include "TextureLoader.h"
#include "TextureInstance.h"
namespace _NL {
	namespace UI {
		class UIImage : public _NL::Core::UI
		{
		public:
			UIImage(_NL::Element::TextureInstance* Texture);
			~UIImage();
			_NL::Element::TextureInstance* Texture;
			glm::vec2 scale = glm::vec2(1.0f, 1.0f);
			glm::vec2 widthHeight;
			std::string ClassName() const override;
		};
	}
}

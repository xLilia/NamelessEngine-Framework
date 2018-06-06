#pragma once
#include "NamelessCore.hpp"
#include "TextureLoader.h"
#include "TextureInstance.h"
#include "CameraObj.h"
namespace _NL {
	namespace UI {
		class UIImage : public _NL::Core::UI
		{
		public:
			UIImage(_NL::Element::TextureInstance* Texture);
			~UIImage();
			
			//void DrawImage(_NL::Object::CameraObj* TargetCam);
			
			_NL::Element::TextureInstance* Texture;
			glm::vec2 scale = glm::vec2(1.0f, 1.0f);
			glm::vec2 widthHeight;
			char* getTypeName() const override;
		};
	}
}

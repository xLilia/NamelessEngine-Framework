#pragma once
#include "NamelessCore.h"
#include "TextureLoader.h"
namespace _NL {
	namespace UI {
		class UIImage : public _NL::Core::UI
		{
		public:
			UIImage();
			UIImage(char* const filepath);
			~UIImage();
			void LoadTexture(char* const filepath);
			GLuint TextureID = NULL;
			glm::vec2 widthHight;
			GLfloat scale = 1;
			std::string ClassName() const override;
		private:
			_NL::Tools::TextureLoader TL;
		};
	}
}

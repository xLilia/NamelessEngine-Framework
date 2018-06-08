#pragma once
#include "NamelessCore.hpp"
#include "TextureInstance.h"
#include <vector>
namespace _NL {
	namespace Element {
		enum TEXTURE_TYPE
		{
			AlbedoMap = 0,
			RoughnessMap = 1,
			MetalnessMap = 2,
			NormalMap = 3,
			AmbientOcclusionMap = 4
		};
		class MaterialInstance : public _NL::Core::Element
		{
		public:

			MaterialInstance();
			~MaterialInstance();

			virtual char* getTypeName() const override;

			_NL::Core::MaterialInstanceData MaterialInstanceData;

			void LoadTexture(TEXTURE_TYPE Texture_type, _NL::Element::TextureInstance* Texture, GLuint MTL_ID);

			//WIP
			//GLboolean IsFlipbook = false;
			//void LoadFlipbookTextureMap(TEXTURE_TYPE Texture_type, const char* filePath, GLuint MTL_ID, GLuint Rows, GLuint Columns, GLuint* AnimationPhase);
		};
	}
}



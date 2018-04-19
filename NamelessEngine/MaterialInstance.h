#pragma once
#include "NamelessCore.h"
#include "TextureLoader.h"
#include "MTLfileReader.h"
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

			std::string ClassName() const override;

			std::vector<_NL::Core::MaterialInstanceData> MaterialInstanceData;

			void LoadTextureMap(TEXTURE_TYPE Texture_type, const char* filePath, GLuint MTL_ID);
			void AddNew_Material();


			GLboolean IsFlipbook = false;
			void LoadFlipbookTextureMap(TEXTURE_TYPE Texture_type, const char* filePath, GLuint MTL_ID, GLuint Rows, GLuint Columns, GLuint* AnimationPhase);

		private:
			_NL::Tools::TextureLoader TL;
		};
	}
}



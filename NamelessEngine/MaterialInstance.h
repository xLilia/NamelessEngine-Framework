#pragma once
#include "NamelessCore.h"
#include "TextureLoader.h"
#include "MTLfileReader.h"
#include <vector>
namespace _NL {
	namespace Element {
		class MaterialInstance : public _NL::Core::Element
		{
		public:
			MaterialInstance();
			~MaterialInstance();

			void LoadAlbedoMap(const char* filePath, GLuint MTL_ID);
			void LoadRoughnessMap(const char* filePath, GLuint MTL_ID);
			void LoadMetalnessMap(const char* filePath, GLuint MTL_ID);
			void LoadNormalMap(const char* filePath, GLuint MTL_ID);
			void LoadAmbientOcclusionMap(const char* filePath, GLuint MTL_ID);

			std::string ClassName() const override;

			std::vector<_NL::Core::MaterialInstanceData> MaterialInstanceData;

			void AddNew_Material();

		private:
			_NL::Tools::TextureLoader TL;
		};
	}
}



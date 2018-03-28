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
			MaterialInstance(const char* filePath);
			~MaterialInstance();

			//int LoadMTLFile(const char* filePath);
			std::string ClassName() const override;

			std::vector<_NL::Core::MaterialInstanceData> MaterialInstanceData;

			void AddNew_Material();

		private:
			MTLfileReader MTLF;
		};
	}
}



#pragma once
#include "NamelessCore.h"
#include "TextureLoader.h"
#include "MTLfileReader.h"
#include <vector>
namespace _NL {
	namespace Element {
		class MaterialObj : public _NL::Core::Element
		{
		public:
			MaterialObj();
			MaterialObj(const char* filePath);
			~MaterialObj();

			int LoadMTLFile(const char* filePath);
			std::string ClassName() const override;

			std::vector<_NL::Core::MTLdata> MTLData;
			std::vector<GLuint> MTLTexIds;

		private:
			MTLfileReader MTLF;
		};
	}
}


#pragma once
#include "NamelessCore.h"
#include "OBJfileReader.h"

namespace _NL {
	namespace Element {
		class MeshObj : public _NL::Core::Element
		{
		public:
			MeshObj(std::string name);
			MeshObj(std::string name, const char * filePath);
			~MeshObj();

			int LoadOBJFile(const char* filePath);
			_NL::Core::MeshData MeshData;
			std::vector<_NL::Core::vIndices> Indices;
			std::string ClassName() const override;
		private:
			OBJfileReader OBJF;
		};
	}
}


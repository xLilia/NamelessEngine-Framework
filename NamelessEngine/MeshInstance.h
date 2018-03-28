#pragma once
#include "NamelessCore.h"
#include "OBJfileReader.h"

namespace _NL {
	namespace Element {
		class MeshInstance : public _NL::Core::Element
		{
		public:
			MeshInstance();
			MeshInstance(const char * filePath);
			~MeshInstance();

			int LoadOBJFile(const char* filePath);
			_NL::Core::MeshData MeshData;
			std::vector<_NL::Core::vIndices> Indices;
			std::string ClassName() const override;
		private:
			OBJfileReader OBJF;
		};
	}
}


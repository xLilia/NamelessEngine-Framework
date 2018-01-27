#pragma once
#include "NamelessCore.h"
#include "ShaderWizard.h"
#include "OBJfileReader.h"

namespace _NL {
	namespace Component {
		class MeshRenderer : public _NL::Core::Component
		{
		public:
			MeshRenderer();
			~MeshRenderer();

			_NL::Core::Mesh Mesh;
			int LoadOBJFile(const char* filePath);
			ShaderWizard Shader;

			///STATES
			bool bactive = true;
			///INFO
			std::string ClassName() const override;
		private:
			OBJfileReader OBJF;
		};
	}
}


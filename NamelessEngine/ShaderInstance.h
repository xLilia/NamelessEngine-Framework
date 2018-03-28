#pragma once
#include"NamelessCore.h"
#include "ShaderWizard.h"

namespace _NL {
	namespace Element {
		class ShaderInstance : public _NL::Core::Element
		{
		public:
			ShaderInstance();
			ShaderInstance(const char * VertexShader, const char * FragmentShader);
			~ShaderInstance();

			GLuint getShaderProgram() const;
			void InstallShader(const char * VertexShader, const char * FragmentShader);
			std::string ClassName() const override;

		private:
			ShaderWizard Shader;
		};
	}
}
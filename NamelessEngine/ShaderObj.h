#pragma once
#include"NamelessCore.h"
#include "ShaderWizard.h"

namespace _NL {
	namespace Element {
		class ShaderObj : public _NL::Core::Element
		{
		public:
			ShaderObj(std::string name);
			ShaderObj(std::string name, const char * VertexShader, const char * FragmentShader);
			~ShaderObj();
			GLuint getShaderProgram() const;
			void InstallShader(const char * VertexShader, const char * FragmentShader);
			
			std::string ClassName() const override;
		private:
			ShaderWizard Shader;
		};
	}
}
#pragma once
#include"NamelessCore.hpp"
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
			void Use();
			void UnUse();
			void InstallShader(const char * VertexShader, const char * FragmentShader);
			virtual char* getTypeName() const override;

		private:
			_NL::Tools::ShaderWizard Shader;
		};
	}
}
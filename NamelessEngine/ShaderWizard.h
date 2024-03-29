#pragma once
#include <iostream>
#include <fstream>
#include <GL\glew.h>
#include <vector>

namespace _NL {
	namespace Tools {
		class ShaderWizard
		{
		public:
			void installShaders(const char * VertexShader = NULL, const char * FragmentShader = NULL);
			void clearPrograms();
			~ShaderWizard();
			std::vector<GLuint> InstlledProgramIDs;
		private:
			std::string readShaderCode(const char * filename);
			bool checkStatus(GLuint objectID, PFNGLGETSHADERIVPROC objectPropertyGetterFunc, PFNGLGETSHADERINFOLOGPROC getInfoLogFunc, GLenum statusType);
			bool checkShaderStatus(GLuint ShaderID);
			bool checkProgramStatus(GLuint ProgramID);
		};
	}
}

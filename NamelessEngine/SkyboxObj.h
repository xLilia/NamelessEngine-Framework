#pragma once
#include "ShaderInstance.h"
#include "GLError.h"
#include "TextureLoader.h"

namespace _NL {
	namespace Object {
		class SkyboxObj
		{
		public:
			SkyboxObj();
			~SkyboxObj();
			_NL::Element::ShaderInstance* SkyboxShader;
			const GLuint vPos_atrib = 0;
			const GLuint CamProjection_matrix = 1;
			const GLuint CamView_matrix = 2;
			GLuint vao;
			GLuint vbo;
			GLuint CubeTex;
			GLuint HDR_EnvironmentTex;
			void createHDREnvironmentMap(const char* file_path);
			void createCubeMap(
				const char* front,
				const char* back,
				const char* top,
				const char* bottom,
				const char* left,
				const char* right);
			void RenderSkybox();

		private:
			_NL::Tools::TextureLoader TL;
			void loadCubeSide(const char* file_path, GLenum gl_side_target);
			
		};
	}
}



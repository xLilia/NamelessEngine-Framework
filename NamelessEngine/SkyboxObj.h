#pragma once
#include "ShaderInstance.h"
#include "GLError.h"
#include "TextureLoader.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace _NL {
	namespace Object {
		class SkyboxObj
		{
		public:
			SkyboxObj();
			~SkyboxObj();
			_NL::Element::ShaderInstance* SkyboxShader;
			_NL::Element::ShaderInstance* HDRImageShader;
			_NL::Element::ShaderInstance* EnvironmentShader;
			const GLuint vPos_atrib = 0;
			const GLuint CamProjectionMatrix_uniform = 1;
			const GLuint CamViewMatrix_uniform = 2;

			GLuint vao;
			GLuint vbo;
			GLuint SkyboxCubeMap = NULL;
			GLuint EnvironmentMap = NULL;
			GLuint IrradienceMap = NULL;
			void createEnvironment(const char* file_path, GLuint resolution = 512);
			void createSkybox(
				const char* front,
				const char* back,
				const char* top,
				const char* bottom,
				const char* left,
				const char* right);
			void createSkybox(const char* file_path, GLuint resolution = 1024);
			void RenderSkybox();
		private:
			void _createEnvironment(const char* file_path, GLuint resolution, bool bIsSkybox = false, bool bIsIrradiance = false);
			void Render1x1Cube();
			_NL::Tools::TextureLoader TL;
			void loadCubeSide(const char* file_path, GLenum gl_side_target);
			
		};
	}
}



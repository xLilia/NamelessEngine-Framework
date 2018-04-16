#include "SkyboxObj.h"

_NL::Object::SkyboxObj::SkyboxObj() 
{

	//CubePrimitive
	GLfloat cube1x1[] = {
		-1.0f, +1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		+1.0f, -1.0f, -1.0f,
		+1.0f, -1.0f, -1.0f,
		+1.0f, +1.0f, -1.0f,
		-1.0f, +1.0f, -1.0f,

		-1.0f, -1.0f, +1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, +1.0f, -1.0f,
		-1.0f, +1.0f, -1.0f,
		-1.0f, +1.0f, +1.0f,
		-1.0f, -1.0f, +1.0f,

		+1.0f, -1.0f, -1.0f,
		+1.0f, -1.0f, +1.0f,
		+1.0f, +1.0f, +1.0f,
		+1.0f, +1.0f, +1.0f,
		+1.0f, +1.0f, -1.0f,
		+1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, +1.0f,
		-1.0f, +1.0f, +1.0f,
		+1.0f, +1.0f, +1.0f,
		+1.0f, +1.0f, +1.0f,
		+1.0f, -1.0f, +1.0f,
		-1.0f, -1.0f, +1.0f,

		-1.0f, +1.0f, -1.0f,
		+1.0f, +1.0f, -1.0f,
		+1.0f, +1.0f, +1.0f,
		+1.0f, +1.0f, +1.0f,
		-1.0f, +1.0f, +1.0f,
		-1.0f, +1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, +1.0f,
		+1.0f, -1.0f, -1.0f,
		+1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, +1.0f,
		+1.0f, -1.0f, +1.0f
	};

	glCreateBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(GLfloat), &cube1x1[0], GL_STATIC_DRAW);

	check_gl_error();

	glCreateVertexArrays(1, &vao);
	glEnableVertexArrayAttrib(vao, vPos_atrib);

	glVertexArrayAttribBinding(vao, vPos_atrib, 0);
	glVertexArrayAttribFormat(vao, vPos_atrib, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 0);

	glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(GLfloat) * 3);

	check_gl_error();
}

void _NL::Object::SkyboxObj::_createEnvironment(const char * file_path, GLuint resolution, GLfloat BlurLevel, bool bIsSkybox, bool bIsIrradiance, bool bIsPreFiltering)
{
	GLuint TexMap = NULL;
	//Generate Environment Map s0i0p1
	if (!bIsIrradiance && !bIsPreFiltering) {
		EnvRes = resolution;
		EnvBlur = BlurLevel;
		TexMap = this->TL.GenerateTexure(file_path, false);
	}
	else {
		TexMap = EnvironmentMap;
	}
	
	//Init FrameBuffer && RenderBuffer
	GLuint captureFBO, captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);

	if (!bIsPreFiltering) {
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, resolution, resolution);
	}
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	//Generate CubeTexture
	if (bIsSkybox) {
		glDeleteTextures(1, &SkyboxMap);
		glGenTextures(1, &SkyboxMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, SkyboxMap);
	}else if(!bIsIrradiance && !bIsPreFiltering ){
		glDeleteTextures(1, &EnvironmentMap);
		glGenTextures(1, &EnvironmentMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, EnvironmentMap);
	}else if(bIsIrradiance){
		glDeleteTextures(1, &IrradienceMap);
		glGenTextures(1, &IrradienceMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, IrradienceMap);
	}else if(bIsPreFiltering){
		glDeleteTextures(1, &PreFilterMap);
		glGenTextures(1, &PreFilterMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, PreFilterMap);
	}

	//Create 6 2DTextures (Cube Faces)
	for (int i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
			resolution, resolution, 0, GL_RGB, GL_FLOAT, nullptr);
	}

	//Set Texture Properties
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	if (bIsPreFiltering) {
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //PREFILTER RENDER
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP); 
	}
	else {
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //ANY OTHER RENDER
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

		

	//Prepare Shader Uniforms
	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	//SetUpShader
	if (!bIsIrradiance && !bIsPreFiltering) {
		HDRImageShader->Use(); //SKYBOX & ENVIRONMENT RENDER
	}
	else if(bIsIrradiance && !bIsPreFiltering){
		EnvironmentShader->Use(); //IRRADIANCE RENDER
	}
	else {
		PreFilterShader->Use();  //PREFILTER RENDER
	}
		
	glUniformMatrix4fv(CamProjectionMatrix_uniform, 1, false, glm::value_ptr(captureProjection));
	
	//BindTargetTexture
	glActiveTexture(GL_TEXTURE0);
	if (bIsSkybox) {
		glBindTexture(GL_TEXTURE_2D, TexMap);
	}
	else {
		glBindTexture(GL_TEXTURE_CUBE_MAP, TexMap);
	}
	
	//SetUpRender
	glViewport(0, 0, resolution, resolution); //set viewport to the capture dimensions
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

	if (bIsPreFiltering) {
		GLuint maxMipLevels = 5;
		for (GLuint mip = 0; mip < maxMipLevels; ++mip) {
			// reisze framebuffer according to mip-level size.
			GLuint mipWidth  = resolution * std::pow(0.5, mip);
			GLuint mipHeight = resolution * std::pow(0.5, mip);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
			glViewport(0, 0, mipWidth, mipHeight);

			GLfloat roughness = (GLfloat)mip / (GLfloat)(maxMipLevels - 1);
			glUniform1f(MipRoughness_uniform, roughness);
			glUniform1f(resolution_uniform, EnvRes);

			for (unsigned int i = 0; i < 6; ++i){
				glUniformMatrix4fv(CamViewMatrix_uniform, 1, false, glm::value_ptr(captureViews[i]));
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, PreFilterMap, mip);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				Render1x1Cube();
			}
		}
	}
	else {
		//StartRender
		for (int i = 0; i < 6; ++i) {
			glUniformMatrix4fv(CamViewMatrix_uniform, 1, false, glm::value_ptr(captureViews[i]));
			if (!bIsSkybox && !bIsIrradiance) {
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, EnvironmentMap, 0);
			}
			else if (!bIsIrradiance) {
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, SkyboxMap, 0);
			}
			else {
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, IrradienceMap, 0);
			}
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			Render1x1Cube();
		}
	}

	if (bIsSkybox) {
		check_gl_error();
		std::cout << "SkyboxMap of file \"" << file_path << "\" created successfully!" << std::endl;
		return;
	}
	if (!bIsIrradiance && !bIsPreFiltering) {
		check_gl_error();
		std::cout << "EnvironmentMap of file \"" << file_path << "\" created successfully!" << std::endl;
		_createEnvironment(file_path, resolution / 16, BlurLevel, false, true, false); //IRRADIANCE RENDER
		
	}
	if(bIsIrradiance && !bIsPreFiltering){
		check_gl_error();
		std::cout << "IrradianceMap of file \"" << file_path << "\" created successfully!" << std::endl;
		_createEnvironment(file_path, resolution * 4, BlurLevel, false, false, true); //IRRADIANCE RENDER
	}
	if (bIsPreFiltering) {
		check_gl_error();
		std::cout << "PreFilterMap of file \"" << file_path << "\" created successfully!" << std::endl;
		
		//BRDF 2D LUT

		glDeleteTextures(1, &BRDF_2D_LUTMap);
		glGenTextures(1, &BRDF_2D_LUTMap);
		glBindTexture(GL_TEXTURE_2D, BRDF_2D_LUTMap);

		// pre-allocate enough memory for the LUT texture.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//RenderQuad 

		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, BRDF_2D_LUTMap, 0);

		_NL::Core::RenderScreenQuad(0, 0, 512, 512, BRDFShader->getShaderProgram());
	}
	
	//End Render
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
	glDeleteFramebuffers(1, &captureFBO);
	glDeleteRenderbuffers(1, &captureRBO);
}



void _NL::Object::SkyboxObj::createEnvironment(const char * file_path, GLuint resolution, GLfloat BlurLevel)
{
	_createEnvironment(file_path, resolution, BlurLevel, false, false, false); //ENVIRONMENT RENDER
}

void _NL::Object::SkyboxObj::createSkybox(const char* file_path, GLuint resolution) {
	_createEnvironment(file_path, resolution, EnvBlur, true, false, false); //SKYBOX RENDER
}


void _NL::Object::SkyboxObj::createSkybox(const char * front, const char * back, const char * top, const char * bottom, const char * left, const char * right)
{
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &SkyboxMap);

	//LOAD SIDES
	loadCubeSide(front, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
	loadCubeSide(back, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	loadCubeSide(top, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	loadCubeSide(bottom, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	loadCubeSide(left, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	loadCubeSide(right, GL_TEXTURE_CUBE_MAP_POSITIVE_X);

	//FORMAT CUBE MAP TEX
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
}

void _NL::Object::SkyboxObj::loadCubeSide(const char * file_path, GLenum gl_side_target)
{
	//Check if img is loaded
	glBindTexture(GL_TEXTURE_CUBE_MAP, SkyboxMap);
	if (TL.LoadImage(file_path) != 0) {
		fprintf(stderr,
			"WARNING: image %s not Loaded\n",
			file_path);
	}
	//Chech if img dimentions is pow of 2
	sf::Vector2u pxs = TL.LoadedTexture.getSize();
	if ((pxs.x & (pxs.x - 1)) != 0 || (pxs.y & (pxs.y - 1)) != 0) {
		fprintf(stderr,
			"WARNING: image %s is not power-of-2 dimensions\n",
			file_path);
	}
	// copy image data into 'target' side of cube map
	glTexImage2D(
		gl_side_target,
		0,
		GL_RGBA,
		pxs.x,
		pxs.y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,	
		TL.LoadedTexture.getPixelsPtr()
	);
}

void _NL::Object::SkyboxObj::RenderSkybox()
{
	if (this->SkyboxMap == NULL && this->PreFilterMap != NULL) {
		glDepthMask(GL_FALSE);
		//glUseProgram in GameManager	
		glUniform1f(MipLVL_uniform, EnvBlur);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, PreFilterMap);
		Render1x1Cube();
		glUseProgram(0);
		glDepthMask(GL_TRUE);
		check_gl_error();
	}else{
		glDepthMask(GL_FALSE);
		//glUseProgram in GameManager
		glUniform1f(MipLVL_uniform, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, SkyboxMap);
		Render1x1Cube();
		glUseProgram(0);
		glDepthMask(GL_TRUE);
		check_gl_error();
	}
}

void _NL::Object::SkyboxObj::Render1x1Cube(){
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

_NL::Object::SkyboxObj::~SkyboxObj()
{
}


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
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(GLfloat), &cube1x1, GL_STATIC_DRAW);

	check_gl_error_full();

	glCreateVertexArrays(1, &vao);
	glEnableVertexArrayAttrib(vao, vPos_atrib);

	glVertexArrayAttribBinding(vao, vPos_atrib, 0);
	glVertexArrayAttribFormat(vao, vPos_atrib, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 0);

	glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(GLfloat) * 3);

	check_gl_error_full();
}

void _NL::Object::SkyboxObj::createEnvironment(const char * file_path, GLuint resolution, bool bIsSkybox, bool bIsIrradiance)
{
	GLuint TexMap = NULL;
	//Generate Environment Map
	if (!bIsIrradiance) {
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

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, resolution, resolution);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	//Generate CubeTexture
	if (!bIsSkybox && !bIsIrradiance) {
		glGenTextures(1, &EnvironmentMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, EnvironmentMap);
	}
	else if(!bIsIrradiance){
		glGenTextures(1, &SkyboxCubeMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, SkyboxCubeMap);
	}
	else {
		glGenTextures(1, &IrradienceMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, IrradienceMap);
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
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
	if (!bIsIrradiance) {
		HDRImageShader->Use();
	}
	else {
		EnvironmentShader->Use();
	}
		
	glUniformMatrix4fv(CamProjection_matrix, 1, false, glm::value_ptr(captureProjection));
	
	//BindTargetTexture
	glActiveTexture(GL_TEXTURE0);
	if (!bIsIrradiance) {
		glBindTexture(GL_TEXTURE_2D, TexMap);
	}
	else {
		glBindTexture(GL_TEXTURE_CUBE_MAP, TexMap);
	}
	
	//SetUpRender
	glViewport(0, 0, resolution, resolution); //set viewport to the capture dimensions
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

	//StartRender
	for (int i = 0; i < 6; ++i) {
		glUniformMatrix4fv(CamView_matrix, 1, false, glm::value_ptr(captureViews[i]));
		if (!bIsSkybox && !bIsIrradiance) {
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, EnvironmentMap, 0);
		}
		else if(!bIsIrradiance){
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, SkyboxCubeMap, 0);
		}
		else {
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, IrradienceMap, 0);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Render1x1Cube();
	}
	//End Render
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);

	if (bIsSkybox) {
		return;
	}
	if(!bIsIrradiance)
		createEnvironment("", resolution/16, false, true);
}

void _NL::Object::SkyboxObj::createSkybox(const char* file_path, GLuint resolution) {
	createEnvironment(file_path, resolution, true);
}

void _NL::Object::SkyboxObj::createSkybox(const char * front, const char * back, const char * top, const char * bottom, const char * left, const char * right)
{
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &SkyboxCubeMap);

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
	glBindTexture(GL_TEXTURE_CUBE_MAP, SkyboxCubeMap);
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
	if (this->SkyboxCubeMap == NULL && this->IrradienceMap != NULL) {
		glDepthMask(GL_FALSE);
		//glUseProgram in WindowManager
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, IrradienceMap);
		Render1x1Cube();
		glUseProgram(0);
		glDepthMask(GL_TRUE);
		check_gl_error_full();
	}else{
		glDepthMask(GL_FALSE);
		//glUseProgram in WindowManager
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, SkyboxCubeMap);
		Render1x1Cube();
		glUseProgram(0);
		glDepthMask(GL_TRUE);
		check_gl_error_full();
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


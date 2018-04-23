#include "CameraObj.h"

_NL::Object::CameraObj::CameraObj(std::string name, GLsizei RenderWindowWidth, GLsizei RenderWindowHeight, GLsizei RenderWindowX, GLsizei RenderWindowY, GLfloat FOV, GLfloat NearPlane, GLfloat FarPlane, GLfloat RenderScaleRatio, GLuint nMultisamples, GLuint nRenderTextures, bool HasPingPongShader, GLuint PingPongIterations)
{
	this->name = name;
	this->FOV = FOV;
	this->NearPlane = NearPlane;
	this->FarPlane = FarPlane;
	this->RenderWindowPos.x = RenderWindowX;
	this->RenderWindowPos.y = RenderWindowY;
	this->RenderWindowSize.x = RenderWindowWidth;
	this->RenderWindowSize.y = RenderWindowHeight;
	this->RenderScaleRatio = RenderScaleRatio;
	this->nMultisamples = nMultisamples;
	this->nRenderTextures = nRenderTextures;
	this->HasPingPongShader = HasPingPongShader;
	this->PingPongIterations = PingPongIterations;
	updateAudioListenerWithCamTransform();
}

void _NL::Object::CameraObj::updateAudioListenerWithCamTransform()
{
	updateAudioListenerPosition(Position);
	updateAudioListenerDirection(LookAt);
	updateAudioListenerRotation(UpAxis);
}

void _NL::Object::CameraObj::updateAudioListenerPosition(glm::vec3 pos)
{
	AudioListener.setPosition(pos.x, pos.y, pos.z);
}

void _NL::Object::CameraObj::updateAudioListenerDirection(glm::vec3 dir)
{
	AudioListener.setDirection(dir.x, dir.y, dir.z);
}

void _NL::Object::CameraObj::updateAudioListenerRotation(glm::vec3 upVec)
{
	AudioListener.setUpVector(upVec.x, upVec.y, upVec.z);
}

glm::mat4 _NL::Object::CameraObj::getWorldToViewMatrix() const
{
	return glm::lookAt(Position, Position + LookAt, UpAxis);
}

glm::mat4 _NL::Object::CameraObj::getViewMatrix() const
{
	return glm::lookAt(glm::vec3(), glm::vec3() + LookAt, UpAxis);
}

glm::mat4 _NL::Object::CameraObj::getProjectionMatrix() const
{
	return glm::perspective(glm::radians(FOV), (GLfloat)RenderWindowSize.x / (GLfloat)RenderWindowSize.y, NearPlane, FarPlane);
}

void _NL::Object::CameraObj::GenerateFrameBuffers() {

	//---------------------------------------------------------------------------------
	//CLEAN & UPDATE
	//---------------------------------------------------------------------------------
	GLboolean success = true;
	glDeleteTextures(1, &PostProcessing_ColorTexture);
	glDeleteTextures(1, &PostProcessing_DepthTexture);
	glDeleteFramebuffers(1, &PostProcessingFrameBuffer);

	if(ColorTextures.size()>0)
		glDeleteTextures(ColorTextures.size(), &ColorTextures[0]);
	ColorTextures.clear();

	glDeleteTextures(1, &DepthTexture);
	glDeleteFramebuffers(1, &SceneRenderFrameBuffer);

	//glDeleteRenderbuffers(1, &rboDepth);
	
	glDeleteTextures(2, &pingPongTexture[0]);
	glDeleteFramebuffers(2, &pingPongFBO[0]);

	if (HasPingPongShader && nRenderTextures < 2) {
		nRenderTextures = 2;
	}

	if (nMultisamples == 0) {

		//---------------------------------------------------------------------------------
		//ColorTexture
		//---------------------------------------------------------------------------------
		for (GLuint i = 0; i < nRenderTextures; i++) {
			GLuint ColorTexture;
			glCreateTextures(GL_TEXTURE_2D, 1, &ColorTexture);
			glBindTexture(GL_TEXTURE_2D, ColorTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA16F, //high dynamic range (HDR) 
				RenderWindowSize.x * RenderScaleRatio,
				RenderWindowSize.y * RenderScaleRatio,
				0,
				GL_RGBA,
				GL_FLOAT,
				NULL);
			glBindTexture(GL_TEXTURE_2D, 0);
			check_gl_error();
			ColorTextures.push_back(ColorTexture);
		}
		//---------------------------------------------------------------------------------
		//DepthTexture
		//---------------------------------------------------------------------------------
	
		glCreateTextures(GL_TEXTURE_2D, 1, &DepthTexture);
		glBindTexture(GL_TEXTURE_2D, DepthTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_DEPTH_COMPONENT,
			RenderWindowSize.x * RenderScaleRatio,
			RenderWindowSize.y * RenderScaleRatio,
			0,
			GL_DEPTH_COMPONENT,
			GL_UNSIGNED_BYTE,
			NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
		check_gl_error();


		//---------------------------------------------------------------------------------
		//Render_FrameBuffers
		//---------------------------------------------------------------------------------

		glCreateFramebuffers(1, &SceneRenderFrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, SceneRenderFrameBuffer);
		for (GLuint i = 0; i < nRenderTextures; i++) {
			glFramebufferTexture2D(
				GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0+i,
				GL_TEXTURE_2D,
				ColorTextures[i],
				0);
		}
		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_DEPTH_ATTACHMENT,
			GL_TEXTURE_2D,
			DepthTexture,
			0);
		check_gl_error();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		check_gl_error();
	}
	else {

		glEnable(GL_MULTISAMPLE);

		//---------------------------------------------------------------------------------
		//MULTISAMPLE_ColorTexture
		//---------------------------------------------------------------------------------
		for (GLuint i = 0; i < nRenderTextures; i++) {
			GLuint ColorTexture;
			glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &ColorTexture);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, ColorTexture);
			glTexImage2DMultisample(
				GL_TEXTURE_2D_MULTISAMPLE,
				nMultisamples,
				GL_RGBA16F, //high dynamic range (HDR) 
				RenderWindowSize.x * RenderScaleRatio,
				RenderWindowSize.y * RenderScaleRatio,
				GL_TRUE);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
			check_gl_error();
			ColorTextures.push_back(ColorTexture);
		}

		//---------------------------------------------------------------------------------
		//MULTISAMPLE_DepthTexture
		//---------------------------------------------------------------------------------

		glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &DepthTexture);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, DepthTexture);
		glTexImage2DMultisample(
			GL_TEXTURE_2D_MULTISAMPLE,
			nMultisamples,
			GL_DEPTH_COMPONENT,
			RenderWindowSize.x * RenderScaleRatio,
			RenderWindowSize.y * RenderScaleRatio,
			GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		check_gl_error();


		//---------------------------------------------------------------------------------
		//MULTISAMPLE_Render_FrameBuffer
		//---------------------------------------------------------------------------------

		glCreateFramebuffers(1, &SceneRenderFrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, SceneRenderFrameBuffer);
		for (GLuint i = 0; i < nRenderTextures; i++) {
			glFramebufferTexture2D(
				GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0 + i,
				GL_TEXTURE_2D_MULTISAMPLE,
				ColorTextures[i],
				0);
		}
		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_DEPTH_ATTACHMENT,
			GL_TEXTURE_2D_MULTISAMPLE,
			DepthTexture,
			0);
		check_gl_error();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		check_gl_error();
	}

	//---------------------------------------------------------------------------------
	//RENDER BUFFER & RENDER_ATACHMENTS
	//---------------------------------------------------------------------------------
	
	//glGenRenderbuffers(1, &rboDepth);
	//glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	//glRenderbufferStorage(
	//	GL_RENDERBUFFER,
	//	GL_DEPTH_COMPONENT,
	//	RenderWindowSize.x * RenderScaleRatio,
	//	RenderWindowSize.y * RenderScaleRatio
	//);
	//glBindFramebuffer(GL_FRAMEBUFFER, SceneRenderFrameBuffer);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);

	GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
		check_gl_error();
		std::cout << "Framebuffer not complete: " << fboStatus << std::endl;
		success = false;
	}
		
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	check_gl_error();

	

	//---------------------------------------------------------------------------------
	//PostProcessing_ColorTexture
	//---------------------------------------------------------------------------------

	glCreateTextures(GL_TEXTURE_2D, 1, &PostProcessing_ColorTexture);
	glBindTexture(GL_TEXTURE_2D, PostProcessing_ColorTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA16F, //high dynamic range (HDR) 
		RenderWindowSize.x * RenderScaleRatio,
		RenderWindowSize.y * RenderScaleRatio,
		0,
		GL_RGBA,
		GL_FLOAT,
		NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	check_gl_error();


	//---------------------------------------------------------------------------------
	//PostProcessing_DepthTexture
	//---------------------------------------------------------------------------------

	glCreateTextures(GL_TEXTURE_2D, 1, &PostProcessing_DepthTexture);
	glBindTexture(GL_TEXTURE_2D, PostProcessing_DepthTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_DEPTH_COMPONENT,
		RenderWindowSize.x * RenderScaleRatio,
		RenderWindowSize.y * RenderScaleRatio,
		0,
		GL_DEPTH_COMPONENT,
		GL_UNSIGNED_BYTE,
		NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	check_gl_error();


	//---------------------------------------------------------------------------------
	//PostProcessing_FrameBuffer
	//---------------------------------------------------------------------------------

	glCreateFramebuffers(1, &PostProcessingFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, PostProcessingFrameBuffer);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		PostProcessing_ColorTexture,
		0);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D,
		PostProcessing_DepthTexture,
		0);
	check_gl_error();

	fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
		check_gl_error();
		std::cout << "Framebuffer not complete: " << fboStatus << std::endl;
		success = false;
	}
	

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	check_gl_error();
	
	//---------------------------------------------------------------------------------
	//PING_PONG SHADERS?
	//---------------------------------------------------------------------------------

	if (HasPingPongShader) {
		glGenFramebuffers(2, pingPongFBO);
		glGenTextures(2, pingPongTexture);
		for (GLuint i = 0; i < 2; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingPongFBO[i]);
			glBindTexture(GL_TEXTURE_2D, pingPongTexture[i]);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA16F,
				RenderWindowSize.x * RenderScaleRatio,
				RenderWindowSize.y * RenderScaleRatio,
				0,
				GL_RGBA,
				GL_FLOAT,
				NULL
			);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(
				GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D,
				pingPongTexture[i],
				0
			);
			fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
				check_gl_error();
				std::cout << "Framebuffer not complete: " << fboStatus << std::endl;
				success = false;
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		check_gl_error();
	}
	/*else if (HasPingPongShader && nMultisamples > 0) {
		glGenFramebuffers(2, pingPongFBO);
		glGenTextures(2, pingPongTexture);
		for (GLuint i = 0; i < 2; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingPongFBO[i]);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, pingPongTexture[i]);
			glTexImage2DMultisample(
				GL_TEXTURE_2D_MULTISAMPLE,
				nMultisamples,
				GL_RGB16F,
				RenderWindowSize.x * RenderScaleRatio,
				RenderWindowSize.y * RenderScaleRatio,
				GL_TRUE
			);
			glFramebufferTexture2D(
				GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D_MULTISAMPLE,
				pingPongTexture[i],
				0
			);
			fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
				check_gl_error();
				std::cout << "Framebuffer not complete: " << fboStatus << std::endl;
				success = false;
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		check_gl_error();
	}*/
	
	if (success) {
		std::cout << name.c_str() << " 's Framebuffers Installation Successful!" << std::endl;
	}
	else {
		std::cout << name.c_str() << " 's Framebuffers Installation UnSuccessful X_X !" << std::endl;
	}
	
}

void _NL::Object::CameraObj::ClearCurrentBuffer()
{
	glClearColor(ClearScreenColor.x, ClearScreenColor.y, ClearScreenColor.z, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void _NL::Object::CameraObj::PrepareToRenderScene()
{	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, SceneRenderFrameBuffer);
	if (ColorTextures.size() > 0) {
		GLenum* atachments = new GLenum[ColorTextures.size()];
		for (GLuint i = 0; i < nRenderTextures; i++) {
			atachments[i] = GL_COLOR_ATTACHMENT0 + i;
		}
		glDrawBuffers(nRenderTextures, atachments);
	}
	glViewport(
		RenderWindowPos.x,
		RenderWindowPos.y,
		RenderWindowSize.x * RenderScaleRatio,
		RenderWindowSize.y * RenderScaleRatio
	);
	ClearCurrentBuffer();
	check_gl_error();
	//Ready for render...
}

void _NL::Object::CameraObj::DisplayOnScreen(GLuint camID, GLuint* aditionalTextures)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, SceneRenderFrameBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, PostProcessingFrameBuffer);

	check_gl_error();

	glBlitFramebuffer(
		RenderWindowPos.x,
		RenderWindowPos.y,
		RenderWindowSize.x * RenderScaleRatio,
		RenderWindowSize.y * RenderScaleRatio,
		RenderWindowPos.x,
		RenderWindowPos.y,
		RenderWindowSize.x * RenderScaleRatio,
		RenderWindowSize.y * RenderScaleRatio,
		GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
		GL_NEAREST
	);

	check_gl_error();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	ClearCurrentBuffer();

	glDisable(GL_DEPTH_TEST);

	PostProcessingShader->Use();

	GLuint texLocation = 0;
	glUniform1i(texLocation, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, PostProcessing_ColorTexture);
	check_gl_error();
	texLocation++;
	if (aditionalTextures != NULL) {
		for (int i = 0; i < sizeof(aditionalTextures) / sizeof(GLuint); i++) {
			glUniform1i(texLocation + i, texLocation + i);
			glActiveTexture(GL_TEXTURE1 + i);
			glBindTexture(GL_TEXTURE_2D, aditionalTextures[i]);
		}
	}
	check_gl_error();


	_NL::Core::RenderQuad(
		RenderWindowPos.x,
		RenderWindowPos.y,
		RenderWindowSize.x,
		RenderWindowSize.y,
		true,
		PostProcessingShader->getShaderProgram(),
		camID
	);

	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_DEPTH_TEST);
	check_gl_error();
}

std::string _NL::Object::CameraObj::ClassName() const
{
	return "_NL::Object::CameraObj";
}

GLuint _NL::Object::CameraObj::GeneratePingPongTexture()
{
	check_gl_error();
	bool bPingPong = true, first_iteration = true;

	if (first_iteration) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		glBindFramebuffer(GL_READ_FRAMEBUFFER, SceneRenderFrameBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, pingPongFBO[0]);
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		check_gl_error();

		glBlitFramebuffer(
			RenderWindowPos.x,
			RenderWindowPos.y,
			RenderWindowSize.x * RenderScaleRatio,
			RenderWindowSize.y * RenderScaleRatio,
			RenderWindowPos.x,
			RenderWindowPos.y,
			RenderWindowSize.x * RenderScaleRatio,
			RenderWindowSize.y * RenderScaleRatio,
			GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
			GL_NEAREST
		);
	}
	check_gl_error();

	GLint amount = PingPongIterations;
	glUseProgram(0);
	PingPongShader->Use();
	for (GLuint i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingPongFBO[bPingPong]);
		check_gl_error();
		GLuint texLocation = 0;
		glUniform1i(texLocation, 0);
		check_gl_error();
		glUniform1i(1, bPingPong);
		check_gl_error();

			glActiveTexture(GL_TEXTURE0);
			check_gl_error();
			glBindTexture(
				GL_TEXTURE_2D, first_iteration ? pingPongTexture[0] : pingPongTexture[!bPingPong]
			);
		
		check_gl_error();
		_NL::Core::RenderQuad(
			RenderWindowPos.x,
			RenderWindowPos.y,
			RenderWindowSize.x,
			RenderWindowSize.y,
			false
		);
		bPingPong = !bPingPong;
		if (first_iteration) first_iteration = false;
	}
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	check_gl_error();
	return pingPongTexture[!bPingPong];
}

_NL::Object::CameraObj::~CameraObj()
{
}


#include "CameraObj.h"

_NL::Object::CameraObj::CameraObj(std::string name, GLsizei RenderWindowWidth, GLsizei RenderWindowHeight, GLsizei RenderWindowX, GLsizei RenderWindowY, GLfloat FOV, GLfloat NearPlane, GLfloat FarPlane, GLfloat RenderScaleRatio, GLuint nRenderTextures, GLuint nMultisamples)
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
	//this->HasPingPongShader = HasPingPongShader;
	//this->PingPongIterations = PingPongIterations;
	updateAudioListenerWithCamTransform();
}

void _NL::Object::CameraObj::updateAudioListenerWithCamTransform()
{
	updateAudioListenerPosition(TransformCam.Position);
	updateAudioListenerDirection(TransformCam.LookAt);
	updateAudioListenerRotation(TransformCam.UpAxis);
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
	return glm::lookAt(TransformCam.Position, TransformCam.Position + TransformCam.LookAt, TransformCam.UpAxis);
}

glm::mat4 _NL::Object::CameraObj::getViewMatrix() const
{
	return glm::lookAt(glm::vec3(), glm::vec3() + TransformCam.LookAt, TransformCam.UpAxis);
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

	glDeleteTextures(1, &DepthStencilTexture);
	glDeleteTextures(1, &StencilViewTexture);
	
	//G framebuffer

	glDeleteFramebuffers(1, &G_FrameBuffer);
	
	if(ColorTextures.size()>0)
		glDeleteTextures(ColorTextures.size(), &ColorTextures[0]);
	ColorTextures.clear();

	//glDeleteTextures(1, &DepthTexture);

	//FinalQuad framebuffer

	glDeleteFramebuffers(1, &FinalQuad_FrameBuffer);

	if (FinalQuad_ColorTextures.size()>0)
		glDeleteTextures(FinalQuad_ColorTextures.size(), &FinalQuad_ColorTextures[0]);
	ColorTextures.clear();
	
	//glDeleteTextures(1, &FinalQuad_DepthTexture);
	
	//pingpong

	//glDeleteTextures(2, &pingPongTexture[0]);
	//glDeleteFramebuffers(2, &pingPongFBO[0]);

	glEnable(GL_DEPTH_TEST);

	if (nRenderTextures <= 0) {
		nRenderTextures = 1;
	}

	if (nRenderTextures > 8) {
		nRenderTextures = 8;
	}

	//if (HasPingPongShader && nRenderTextures < 2) {
	//	nRenderTextures = 2;
	//}

	//---------------------------------------------------------------------------------
	//DEPTH_STENCIL RENDERBUFFER
	//---------------------------------------------------------------------------------

	//glCreateRenderbuffers(1, &DepthStencilRenderbuffer);
	//glBindRenderbuffer(GL_RENDERBUFFER, DepthStencilRenderbuffer);
	//glRenderbufferStorage(
	//	GL_RENDERBUFFER, 
	//	GL_DEPTH24_STENCIL8, 
	//	RenderWindowSize.x * RenderScaleRatio, 
	//	RenderWindowSize.y * RenderScaleRatio);

	glCreateTextures(GL_TEXTURE_2D, 1, &DepthStencilTexture);
	glBindTexture(GL_TEXTURE_2D, DepthStencilTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexStorage2D(
		GL_TEXTURE_2D,
		1,
		GL_DEPTH24_STENCIL8,
		RenderWindowSize.x * RenderScaleRatio,
		RenderWindowSize.y * RenderScaleRatio);/* ,
		0,
		GL_DEPTH_STENCIL,
		GL_UNSIGNED_INT_24_8,
		NULL);*/
	glBindTexture(GL_TEXTURE_2D, 0);
	check_gl_error();

	glCreateTextures(GL_TEXTURE_2D, 1, &StencilViewTexture);
	glBindTexture(GL_TEXTURE_2D, StencilViewTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureView(
		StencilViewTexture,
		GL_TEXTURE_2D,
		DepthStencilTexture,
		GL_DEPTH24_STENCIL8,
		0, 1, 0, 1);
	check_gl_error();
	glBindTexture(GL_TEXTURE_2D, 0);
	check_gl_error();

	//glTexImage2D(
	//	GL_TEXTURE_2D,
	//	0,
	//	GL_DEPTH24_STENCIL8,
	//	RenderWindowSize.x * RenderScaleRatio,
	//	RenderWindowSize.y * RenderScaleRatio,
	//	0,
	//	GL_DEPTH_STENCIL,
	//	GL_UNSIGNED_INT_24_8,
	//	NULL);



	if (nMultisamples == 0) {

		//---------------------------------------------------------------------------------
		//ColorTexture
		//---------------------------------------------------------------------------------
		for (GLuint i = 0; i < nRenderTextures; i++) {
			GLuint ColorTexture;
			glCreateTextures(GL_TEXTURE_2D, 1, &ColorTexture);
			glBindTexture(GL_TEXTURE_2D, ColorTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
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
	
		//glCreateTextures(GL_TEXTURE_2D, 1, &DepthTexture);
		//glBindTexture(GL_TEXTURE_2D, DepthTexture);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexImage2D(
		//	GL_TEXTURE_2D,
		//	0,
		//	GL_DEPTH24_STENCIL8,
		//	RenderWindowSize.x * RenderScaleRatio,
		//	RenderWindowSize.y * RenderScaleRatio,
		//	0,
		//	GL_DEPTH_STENCIL,
		//	GL_UNSIGNED_INT_24_8,
		//	NULL);
		//glBindTexture(GL_TEXTURE_2D, 0);
		//check_gl_error();


		//---------------------------------------------------------------------------------
		//Render_FrameBuffers
		//---------------------------------------------------------------------------------

		glCreateFramebuffers(1, &G_FrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, G_FrameBuffer);
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
			GL_DEPTH_STENCIL_ATTACHMENT,
			GL_TEXTURE_2D,
			DepthStencilTexture,
			0);
		//glFramebufferRenderbuffer(
		//	GL_FRAMEBUFFER,
		//	GL_DEPTH_STENCIL_ATTACHMENT,
		//	GL_RENDERBUFFER,
		//	DepthStencilRenderbuffer
		//);
		check_gl_error();
	}
	else {

		glEnable(GL_MULTISAMPLE);

		//---------------------------------------------------------------------------------
		//MULTISAMPLE_ColorTexture
		//---------------------------------------------------------------------------------
		//for (GLuint i = 0; i < nRenderTextures; i++) {
		//	GLuint ColorTexture;
		//	glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &ColorTexture);
		//	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, ColorTexture);
		//	glTexImage2DMultisample(
		//		GL_TEXTURE_2D_MULTISAMPLE,
		//		nMultisamples,
		//		GL_RGBA16F, //high dynamic range (HDR) 
		//		RenderWindowSize.x * RenderScaleRatio,
		//		RenderWindowSize.y * RenderScaleRatio,
		//		GL_TRUE);
		//	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		//	check_gl_error();
		//	ColorTextures.push_back(ColorTexture);
		//}

		//---------------------------------------------------------------------------------
		//MULTISAMPLE_DepthTexture
		//---------------------------------------------------------------------------------

		//glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &DepthTexture);
		//glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, DepthTexture);
		//glTexImage2DMultisample(
		//	GL_TEXTURE_2D_MULTISAMPLE,
		//	nMultisamples,
		//	GL_DEPTH_COMPONENT,
		//	RenderWindowSize.x * RenderScaleRatio,
		//	RenderWindowSize.y * RenderScaleRatio,
		//	GL_TRUE);
		//glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		//check_gl_error();


		//---------------------------------------------------------------------------------
		//MULTISAMPLE_Render_FrameBuffer
		//---------------------------------------------------------------------------------

		//glCreateFramebuffers(1, &G_FrameBuffer);
		//glBindFramebuffer(GL_FRAMEBUFFER, G_FrameBuffer);
		//for (GLuint i = 0; i < nRenderTextures; i++) {
		//	glFramebufferTexture2D(
		//		GL_FRAMEBUFFER,
		//		GL_COLOR_ATTACHMENT0 + i,
		//		GL_TEXTURE_2D_MULTISAMPLE,
		//		ColorTextures[i],
		//		0);
		//	check_gl_error();
		//}
		//glFramebufferTexture2D(
		//	GL_FRAMEBUFFER,
		//	GL_DEPTH_STENCIL_ATTACHMENT,
		//	GL_TEXTURE_2D_MULTISAMPLE,
		//	DepthTexture,
		//	0);
		//check_gl_error();
	}

	GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
		check_gl_error();
		std::cout << "Framebuffer not complete: " << fboStatus << std::endl;
		success = false;
	}
	check_gl_error();

	//---------------------------------------------------------------------------------
	//FinalQuad_ColorTexture
	//---------------------------------------------------------------------------------

	for (GLuint i = 0; i < nRenderTextures; i++) {
		GLuint ColorTexture;
		glCreateTextures(GL_TEXTURE_2D, 1, &ColorTexture);
		glBindTexture(GL_TEXTURE_2D, ColorTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
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
		FinalQuad_ColorTextures.push_back(ColorTexture);
	}

	//---------------------------------------------------------------------------------
	//FinalQuad_DepthTexture
	//---------------------------------------------------------------------------------

	//glCreateTextures(GL_TEXTURE_2D, 1, &FinalQuad_DepthTexture);
	//glBindTexture(GL_TEXTURE_2D, FinalQuad_DepthTexture);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexImage2D(
	//	GL_TEXTURE_2D,
	//	0,
	//	GL_DEPTH24_STENCIL8,
	//	RenderWindowSize.x * RenderScaleRatio,
	//	RenderWindowSize.y * RenderScaleRatio,
	//	0,
	//	GL_DEPTH_STENCIL,
	//	GL_UNSIGNED_INT_24_8,
	//	NULL);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//check_gl_error();


	//---------------------------------------------------------------------------------
	//FinalQuad_FrameBuffer
	//---------------------------------------------------------------------------------

	glCreateFramebuffers(1, &FinalQuad_FrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, FinalQuad_FrameBuffer);
	for (GLuint i = 0; i < nRenderTextures; i++) {
		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0+i,
			GL_TEXTURE_2D,
			FinalQuad_ColorTextures[i],
			0);
		check_gl_error();
	}
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_DEPTH_STENCIL_ATTACHMENT,
		GL_TEXTURE_2D,
		DepthStencilTexture,
		0);
	//glFramebufferRenderbuffer(
	//	GL_FRAMEBUFFER,
	//	GL_DEPTH_STENCIL_ATTACHMENT,
	//	GL_RENDERBUFFER,
	//	DepthStencilRenderbuffer
	//	);
	check_gl_error();

	fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
		check_gl_error();
		std::cout << "Framebuffer not complete: " << fboStatus << std::endl;
		success = false;
	}
	check_gl_error();
	
	//---------------------------------------------------------------------------------
	//PING_PONG SHADERS?
	//---------------------------------------------------------------------------------

	//if (HasPingPongShader) {
	//	glGenFramebuffers(2, pingPongFBO);
	//	glGenTextures(2, pingPongTexture);
	//	for (GLuint i = 0; i < 2; i++)
	//	{
	//		glBindFramebuffer(GL_FRAMEBUFFER, pingPongFBO[i]);
	//		glBindTexture(GL_TEXTURE_2D, pingPongTexture[i]);
	//		glTexImage2D(
	//			GL_TEXTURE_2D,
	//			0,
	//			GL_RGBA16F,
	//			RenderWindowSize.x * RenderScaleRatio,
	//			RenderWindowSize.y * RenderScaleRatio,
	//			0,
	//			GL_RGBA,
	//			GL_FLOAT,
	//			NULL
	//		);
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//		glFramebufferTexture2D(
	//			GL_FRAMEBUFFER,
	//			GL_COLOR_ATTACHMENT0,
	//			GL_TEXTURE_2D,
	//			pingPongTexture[i],
	//			0
	//		);
	//		fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	//		if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
	//			check_gl_error();
	//			std::cout << "Framebuffer not complete: " << fboStatus << std::endl;
	//			success = false;
	//		}
	//	}
	//	check_gl_error();
	//}
	
	if (success) {
		std::cout << name.c_str() << " 's Framebuffers Installation Successful!" << std::endl;
	}
	else {
		std::cout << name.c_str() << " 's Framebuffers Installation UnSuccessful X_X !" << std::endl;
	}
	
}

void _NL::Object::CameraObj::ClearCurrentBuffer()
{	
	//glEnable(GL_SCISSOR_TEST);
	glClearColor(ClearScreenColor.x, ClearScreenColor.y, ClearScreenColor.z, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//glDisable(GL_SCISSOR_TEST);
	check_gl_error();
}

void _NL::Object::CameraObj::SetThisViewPort(){
	glViewport(
		0,
		0,
		RenderWindowSize.x * RenderScaleRatio,
		RenderWindowSize.y * RenderScaleRatio
	);
	check_gl_error();
}

void _NL::Object::CameraObj::PrepareToRenderScene()
{

	glBindFramebuffer(GL_FRAMEBUFFER, G_FrameBuffer);
	if (ColorTextures.size() > 0) {
		GLenum* atachments = new GLenum[ColorTextures.size()];
		for (GLuint i = 0; i < nRenderTextures; i++) {
			atachments[i] = GL_COLOR_ATTACHMENT0 + i;
		}
		glDrawBuffers(nRenderTextures, atachments);
		delete[] atachments;
	}
	//this->SetThisViewPort();
	//glScissor(
	//	0,
	//	0,
	//	RenderWindowSize.x * RenderScaleRatio,
	//	RenderWindowSize.y * RenderScaleRatio
	//);
	check_gl_error();
	//Ready for render...
}

void _NL::Object::CameraObj::DisplayOnScreen()
{
	for (GLint nRt = 0; nRt < nRenderTextures; nRt++) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, G_FrameBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FinalQuad_FrameBuffer);
		glReadBuffer(GL_COLOR_ATTACHMENT0 + nRt);
		glDrawBuffer(GL_COLOR_ATTACHMENT0 + nRt);
		
		check_gl_error();
		
		glBlitFramebuffer(
			0,
			0,
			RenderWindowSize.x * RenderScaleRatio,
			RenderWindowSize.y * RenderScaleRatio,
			0,
			0,
			RenderWindowSize.x * RenderScaleRatio,
			RenderWindowSize.y * RenderScaleRatio,
			GL_COLOR_BUFFER_BIT,
			GL_NEAREST
		);
	}

	check_gl_error();

	//glBindFramebuffer(GL_READ_FRAMEBUFFER, G_FrameBuffer);
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	//glBlitFramebuffer(
	//	0,
	//	0,
	//	this->RenderWindowSize.x * this->RenderScaleRatio,
	//	this->RenderWindowSize.y * this->RenderScaleRatio,
	//	0,
	//	0,
	//	this->RenderWindowSize.x * this->RenderScaleRatio,
	//	this->RenderWindowSize.y * this->RenderScaleRatio,
	//	GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT,
	//	GL_NEAREST
	//);
	//
	//check_gl_error();
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	check_gl_error();

	FinalPassShader->Use();

	for (int i = 0; i < nRenderTextures; i++) {
		glUniform1i(i, i);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, FinalQuad_ColorTextures[i]);
		check_gl_error();
	}

	glUniform1i(nRenderTextures, nRenderTextures);
	glActiveTexture(GL_TEXTURE0 + nRenderTextures);
	glBindTexture(GL_TEXTURE_2D, DepthStencilTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_DEPTH_COMPONENT);
	
	glUniform1i(nRenderTextures+1, nRenderTextures+1);
	glActiveTexture(GL_TEXTURE0 + nRenderTextures +1);
	glBindTexture(GL_TEXTURE_2D, StencilViewTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_STENCIL_INDEX);

	check_gl_error();

	_NL::Core::RenderQuad(
		RenderWindowPos.x,
		RenderWindowPos.y,
		RenderWindowSize.x,
		RenderWindowSize.y,
		true,
		FinalPassShader->getShaderProgram()
	);

	for (int i = 0; i < nRenderTextures; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glActiveTexture(GL_TEXTURE0 + nRenderTextures);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0 + nRenderTextures + 1);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	check_gl_error();
}

char* _NL::Object::CameraObj::ClassName() const
{
	return "_NL::Object::CameraObj";
}

/*
GLuint _NL::Object::CameraObj::GeneratePingPongTexture()
{
	check_gl_error();
	bool bPingPong = true, first_iteration = true;

	if (first_iteration) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, G_FrameBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, pingPongFBO[0]);
		glReadBuffer(GL_COLOR_ATTACHMENT1);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		check_gl_error();

		glBlitFramebuffer(
			0,
			0,
			RenderWindowSize.x * RenderScaleRatio,
			RenderWindowSize.y * RenderScaleRatio,
			0,
			0,
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
			0,
			0,
			RenderWindowSize.x,
			RenderWindowSize.y,
			false
		);
		bPingPong = !bPingPong;
		if (first_iteration) first_iteration = false;
	}
	glUseProgram(0);
	check_gl_error();
	return pingPongTexture[!bPingPong];
}
*/



#pragma once
#include "NL.hpp"
class GausianBlurPostProcessingEffect : public _NL::Core::PostProcessingScript
{
public:
	_NL::Element::ShaderInstance* PingPongShader;
	GLuint PingPongIterations = 10;

	_NL::Object::CameraObj * TargetCam;

	void Execute() override;
};

void GausianBlurPostProcessingEffect::Execute()
{
	GLuint* pingPongTexture = new GLuint[2];
	GLuint* pingPongFBO = new GLuint[2];

	glDeleteTextures(2, &pingPongTexture[0]);
	glDeleteFramebuffers(2, &pingPongFBO[0]);

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
			TargetCam->RenderWindowSize.x * TargetCam->RenderScaleRatio,
			TargetCam->RenderWindowSize.y * TargetCam->RenderScaleRatio,
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
		GLuint fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
			check_gl_error();
			std::cout << "Framebuffer not complete: " << fboStatus << std::endl;
			return;
		}
	}
	check_gl_error();

	bool bPingPong = true, first_iteration = true;
	if (first_iteration) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, TargetCameraFramebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, pingPongFBO[0]);
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		check_gl_error();

		glBlitFramebuffer(
			0,
			0,
			TargetCam->RenderWindowSize.x *  TargetCam->RenderScaleRatio,
			TargetCam->RenderWindowSize.y *  TargetCam->RenderScaleRatio,
			0,
			0,
			TargetCam->RenderWindowSize.x *  TargetCam->RenderScaleRatio,
			TargetCam->RenderWindowSize.y *  TargetCam->RenderScaleRatio,
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
			TargetCam->RenderWindowSize.x,
			TargetCam->RenderWindowSize.y,
			false
		);
		bPingPong = !bPingPong;
		if (first_iteration) first_iteration = false;
	}
	glUseProgram(0);
	check_gl_error();

	glBindFramebuffer(GL_READ_FRAMEBUFFER, pingPongFBO[0]);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, TargetCameraFramebuffer);

	glBlitFramebuffer(
		0, 0,
		TargetCam->RenderWindowSize.x * TargetCam->RenderScaleRatio,
		TargetCam->RenderWindowSize.y * TargetCam->RenderScaleRatio,
		0, 0,
		TargetCam->RenderWindowSize.x * TargetCam->RenderScaleRatio,
		TargetCam->RenderWindowSize.y * TargetCam->RenderScaleRatio,
		GL_COLOR_BUFFER_BIT,
		TargetCam->TextureFiltering
	);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDeleteTextures(2, &pingPongTexture[0]);
 	glDeleteFramebuffers(2, &pingPongFBO[0]);
	delete[] pingPongTexture;
	delete[] pingPongFBO;
}

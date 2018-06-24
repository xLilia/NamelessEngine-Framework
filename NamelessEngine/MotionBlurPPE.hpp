#pragma once
#include "NL.hpp"
class MotionBlurPPE : public _NL::Core::PostProcessingScript
{
public:
	GLuint MotionFramebuffer = NULL;
	GLuint MotionTexture = NULL;
	GLuint PreMotionTexture = NULL;
	_NL::Object::CameraObj * TargetCam;
	_NL::Element::ShaderInstance* MotionBlurShader;
	glm::vec2 MotionVect;
	inline void Execute() override;
};

void MotionBlurPPE::Execute()
{

	if (MotionTexture == NULL) {
		glGenTextures(1, &MotionTexture);
		glBindTexture(GL_TEXTURE_2D, MotionTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA32F,
			TargetCam->RenderWindowSize.x * TargetCam->RenderScaleRatio,
			TargetCam->RenderWindowSize.y * TargetCam->RenderScaleRatio,
			0,
			GL_RGBA,
			GL_FLOAT,
			NULL
		);
	}

	if (PreMotionTexture == NULL) {
		glGenTextures(1, &PreMotionTexture);
		glBindTexture(GL_TEXTURE_2D, PreMotionTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA32F,
			TargetCam->RenderWindowSize.x * TargetCam->RenderScaleRatio,
			TargetCam->RenderWindowSize.y * TargetCam->RenderScaleRatio,
			0,
			GL_RGBA,
			GL_FLOAT,
			NULL
		);
	}

	if (MotionFramebuffer == NULL) {
		glGenFramebuffers(1, &MotionFramebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, MotionFramebuffer);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D,
			MotionTexture,
			0
		);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT1,
			GL_TEXTURE_2D,
			PreMotionTexture,
			0
		);
		GLuint fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
			check_gl_error();
			std::cout << "Framebuffer not complete: " << fboStatus << std::endl;
			return;
		}
	}
	glUseProgram(0);
	check_gl_error();

	glm::vec4 PosScale = glm::vec4(
		0,
		0,
		0 + TargetCam->RenderWindowSize.x*TargetCam->RenderScaleRatio,
		0 + TargetCam->RenderWindowSize.y*TargetCam->RenderScaleRatio
	);

	check_gl_error();
	glBindFramebuffer(GL_READ_FRAMEBUFFER, TargetCam->PostProcessingReadyFramebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, MotionFramebuffer);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(
		PosScale.x,
		PosScale.y,
		PosScale.z,
		PosScale.w,
		PosScale.x,
		PosScale.y,
		PosScale.z,
		PosScale.w,
		GL_COLOR_BUFFER_BIT,
		TargetCam->TextureFiltering
	);
	check_gl_error();
	glBindFramebuffer(GL_FRAMEBUFFER, MotionFramebuffer);

	MotionBlurShader->Use();

	GLenum* atachments = new GLenum[2]{ GL_COLOR_ATTACHMENT0 , GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, atachments);
	delete[] atachments;

	check_gl_error();

	glUniform1i(0, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, MotionTexture);

	check_gl_error();

	glUniform1i(1, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, PreMotionTexture);

	glUniform2f(2, MotionVect.x,MotionVect.y);

	check_gl_error();

	_NL::Core::RenderQuad(
		0,
		0,
		TargetCam->RenderWindowSize.x*TargetCam->RenderScaleRatio,
		TargetCam->RenderWindowSize.y*TargetCam->RenderScaleRatio
	);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	check_gl_error();
	glBindFramebuffer(GL_READ_FRAMEBUFFER, MotionFramebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, TargetCam->PostProcessingReadyFramebuffer);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(
		PosScale.x,
		PosScale.y,
		PosScale.z,
		PosScale.w,
		PosScale.x,
		PosScale.y,
		PosScale.z,
		PosScale.w,
		GL_COLOR_BUFFER_BIT,
		TargetCam->TextureFiltering
	);
	check_gl_error();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	check_gl_error();
}
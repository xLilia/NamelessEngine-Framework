#pragma once
#include "NL.hpp"
class NightVisionPPE : public _NL::Core::PostProcessingScript
{
public:
	GLuint NightVisionFramebuffer = NULL;
	GLuint NightVisionTexture = NULL;
	GLuint prevNightVisionTexture = NULL;
	_NL::Object::CameraObj * TargetCam;
	_NL::Element::ShaderInstance* NightVisionShader;
	inline void Execute() override;
};

void NightVisionPPE::Execute()
{
	if (NightVisionTexture == NULL) {
		glGenTextures(1, &NightVisionTexture);
		glBindTexture(GL_TEXTURE_2D, NightVisionTexture);
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

	if (prevNightVisionTexture == NULL) {
		glGenTextures(1, &prevNightVisionTexture);
		glBindTexture(GL_TEXTURE_2D, prevNightVisionTexture);
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

	if (NightVisionFramebuffer == NULL) {
		glGenFramebuffers(1, &NightVisionFramebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, NightVisionFramebuffer);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D,
			NightVisionTexture,
			0
		);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT1,
			GL_TEXTURE_2D,
			prevNightVisionTexture,
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
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, NightVisionFramebuffer);
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
	glBindFramebuffer(GL_FRAMEBUFFER, NightVisionFramebuffer);
	
	NightVisionShader->Use();

	GLenum* atachments = new GLenum[2]{ GL_COLOR_ATTACHMENT0 , GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, atachments);
	delete[] atachments;

	check_gl_error();

	glUniform1i(0, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, NightVisionTexture);

	glUniform1i(1, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, prevNightVisionTexture);

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

	check_gl_error();
	glBindFramebuffer(GL_READ_FRAMEBUFFER, NightVisionFramebuffer);
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
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	check_gl_error();
}
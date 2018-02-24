#include "SkyboxObj.h"

_NL::Object::SkyboxObj::SkyboxObj() 
{
	float points[] = {
		-10.0f,  10.0f, -10.0f,
		-10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,

		-10.0f, -10.0f,  10.0f,
		-10.0f, -10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f,  10.0f,
		-10.0f, -10.0f,  10.0f,

		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,

		-10.0f, -10.0f,  10.0f,
		-10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f, -10.0f,  10.0f,
		-10.0f, -10.0f,  10.0f,

		-10.0f,  10.0f, -10.0f,
		10.0f,  10.0f, -10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		-10.0f,  10.0f,  10.0f,
		-10.0f,  10.0f, -10.0f,

		-10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f,  10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f,  10.0f,
		10.0f, -10.0f,  10.0f
	};

	glCreateBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(GLfloat), &points, GL_STATIC_DRAW);

	check_gl_error_full();

	glCreateVertexArrays(1, &vao);
	glEnableVertexArrayAttrib(vao, vPos_atrib);

	glVertexArrayAttribBinding(vao, vPos_atrib, 0);
	glVertexArrayAttribFormat(vao, vPos_atrib, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 0);

	glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(GLfloat) * 3);

	check_gl_error_full();

}


void _NL::Object::SkyboxObj::createCubeMap(const char * front, const char * back, const char * top, const char * bottom, const char * left, const char * right)
{
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &CubeTex);

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

	glActiveTexture(0);
	
}

void _NL::Object::SkyboxObj::loadCubeSide(const char * file_name, GLenum gl_side_target)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, CubeTex);
	if (TL.LoadImage(file_name) != 0) {
		fprintf(stderr,
			"WARNING: image %s not Loaded\n",
			file_name);
	}
	sf::Vector2u pxs = TL.LoadedImage.getSize();
	if ((pxs.x & (pxs.x - 1)) != 0 || (pxs.y & (pxs.y - 1)) != 0) {
		fprintf(stderr,
			"WARNING: image %s is not power-of-2 dimensions\n",
			file_name);
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
		TL.LoadedImage.getPixelsPtr());
}

void _NL::Object::SkyboxObj::RenderSkybox()
{

	glDepthMask(GL_FALSE);
	//glUseProgram
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, CubeTex);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glUseProgram(0);
	glDepthMask(GL_TRUE);
	check_gl_error_full();
}

_NL::Object::SkyboxObj::~SkyboxObj()
{
}


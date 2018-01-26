#pragma once
#include<GL\glew.h>
#include<glm\glm.hpp>
#include "ShaderWizard.h"

namespace _NLess {

	/*struct Static {};
	struct Dynamic {};
	struct Object : Static, Dynamic {};*/

	
	/*
	*example:
	*_nl::Static* v = new _nl::Vertex;
	*_nl::Dynamic* v = new _nl::Vertex;
	*/

	struct Component {};
	struct Primitive {};

	struct Vertex : Primitive {
		glm::vec3 Pos;
		glm::vec3 Norm;
		glm::vec3 color;
	};

	struct Triangle : Primitive {
		Vertex v[3];
	};

	struct Quad : Primitive {
		Vertex v[4];
	};

	struct Mesh : Primitive {
		Triangle* tris;
		GLuint* indices;
		ShaderWizard* Shader;
	};

	

	
}
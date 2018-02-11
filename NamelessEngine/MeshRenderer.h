#pragma once
#include "NamelessCore.h"
#include "ShaderObj.h"
#include "MeshObj.h"
#include "GLError.h"

const static GLuint Pos_atrib = 0;
const static GLuint Col_atrib = 1;
const static GLuint Norm_atrib = 2;
const static GLuint TexC_atrib = 3;

namespace _NL {
	namespace Component {
		class MeshRenderer : public _NL::Core::Component
		{
		public:
			MeshRenderer();
			~MeshRenderer();

			void initGLObj();

			GLuint vbo; //Vertex Buffer Object
			GLuint ebo;	//Element Buffer Object
			GLuint vao; //Vertex Array Object
			std::vector<GLfloat> VertsBuf;
			std::vector<GLuint> IndicesBuf;
			//std::vector<GLuint> IndicesOffset;
			_NL::Element::MeshObj* Mesh;
			_NL::Element::ShaderObj* Shader;
			///INFO
			std::string ClassName() const override;
		private:
			
			void UnpackData();
		};
	}
}


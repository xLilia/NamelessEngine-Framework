#pragma once
#include "NamelessCore.h"
#include "TextureLoader.h"
#include "ShaderObj.h"
#include "MeshObj.h"
#include "MaterialObj.h"
#include "GLError.h"

namespace _NL {
	namespace Component {
		class MeshRenderer : public _NL::Core::Component
		{
		public:
			MeshRenderer();
			~MeshRenderer();

			void initGLObj();

			const  GLuint Pos_atrib = 0;
			const  GLuint Norm_atrib = 1;
			const  GLuint Col_atrib = 2;
			const  GLuint TexC_atrib = 3;
			//const static GLuint FullTransformMatrix_atrib = 4;
			const  GLuint ModelMatrix_atrib = 4;
			const  GLuint ViewMatrix_atrib = 5;
			const  GLuint ProjectionMatrix_atrib = 6;
			//Model
			//View
			//projection
			GLuint vbo; //Vertex Buffer Object
			GLuint ebo;	//Element Buffer Object
			GLuint vao; //Vertex Array Object
			std::vector<GLfloat> VertsBuf;
			std::vector<GLuint> IndicesBuf;
			_NL::Element::MeshObj* Mesh;
			_NL::Element::ShaderObj* Shader;
			_NL::Element::MaterialObj* Material;
			///INFO
			std::string ClassName() const override;
		private:
			
			void UnpackData();
		};
	}
}


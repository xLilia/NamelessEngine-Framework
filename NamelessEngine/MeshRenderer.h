#pragma once
#include "NamelessCore.hpp"
#include "ShaderInstance.h"
#include "MeshInstance.h"
#include "MaterialInstance.h"
#include "GLError.h"

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
			_NL::Element::MeshInstance* Mesh;
			_NL::Element::ShaderInstance* Shader;
			_NL::Element::MaterialInstance* Material;
			///INFO
			char* ClassName() const override;
			bool bIsUnpacked = false;

			//GLSETTINGS
			GLenum GL_RenderMode = GL_TRIANGLES;
			bool GL_CullFace = false;
			GLenum GL_CullFaceMode = GL_BACK;
			GLenum GL_CullFaceOrientation = GL_CCW;

			void UpdateGLSettings();
		private:		
			void UnpackObjData();
		};
	}
}


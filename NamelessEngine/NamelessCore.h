#pragma once
///ExternalLibs
#include<GL\glew.h>
#include <glm\glm.hpp>
#include <iostream>
#include <vector>


namespace _NL{
	namespace Core {

		//TYPES

		//Object
		class Object {
		public:
			///INFO
			std::string name;
			Object *Parent;
			std::vector<Object*> Childs;
			virtual std::string ClassName() const { return "_NL::Core::Object"; };
			///STATES
			bool bactive = true; 
			bool bstatic = false; 
			///FUNC
			virtual void getInfo() {};
		};
		
		//All the Components that can be added to class: _NL::Object::GameObject 
		class Component {
		public:
			virtual std::string ClassName() const { return "_NL::Core::Component"; };
			bool bactive = true;
		};

		//All the Components that can be added to class: _NL::Core::Component
		class Element {
		public:
			///INFO
			std::string name;
			virtual std::string ClassName() const { return "_NL::Core::Element"; };
		};

		//PRIMITIVES
		struct VertexPos {
			glm::vec3 Pos;		//Position			
		};
		struct VertexNorm {
			glm::vec3 Norm;		//Normal
		};
		struct VertexCol {
			glm::vec3 Col;		//Color
		};
		struct VertexTexCoord {
			glm::vec2 TexCoord;	//TexCoordinates 
		};

		struct vIndices {
			GLuint v[3];
			GLuint vt[3];
			GLuint vn[3];
			GLint MTL_ID;
		};

		struct MTLdata {
			GLfloat Ns;	  //Scalar
			glm::vec3 Ka; //Ambient ColorRGB
			glm::vec3 Kd; //Difuse ColorRGB
			glm::vec3 Ks; //Specular ColorRGB
			glm::vec3 Ke; //Emission ColorRGB

			GLuint MTL_ID;
		};

		struct MeshData {
			std::vector<VertexPos> vPos;
			std::vector<VertexCol> vCol;
			std::vector<VertexNorm> vNorm;
			std::vector<VertexTexCoord> vTexC;
		};

		struct transform {
			glm::vec3 position;
			glm::vec3 rotationAxis;
			GLfloat rotationAngle;
			glm::vec3 scale;
		};

		struct ScreenQuad {
			const GLfloat fullquad_t[8] =
			{
				-1,-1,
				+1,-1,
				+1,+1,
				-1,+1
			};

			const GLuint fullquad_i[4] =
			{
				0,1,2,3
			};
		};


	}
}
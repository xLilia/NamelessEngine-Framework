#pragma once
///ExternalLibs
#include<GL\glew.h>
#include <glm\glm.hpp>
//#include <SFML\Graphics.hpp>
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
		//All the Components that can be added to class:
		//_NL::Object::GameObject 
		class Component {
		public:
			virtual std::string ClassName() const { return "_NL::Core::Component"; };
			bool bactive = true;
		};
		//Part of a Component
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
	}
}
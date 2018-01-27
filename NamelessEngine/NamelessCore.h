#pragma once
///ExternalLibs
#include<GL\glew.h>
#include <glm\glm.hpp>
//#include <SFML\Graphics.hpp>
#include <iostream>


namespace _NL{
	namespace Core {

		//TYPES

		//All the Components that can be added to class:
		//_NL::Object::GameObject 
		class Component {
		public:
			virtual std::string ClassName() const { return "_NL::Core::Component"; };
		};
		//Object
		class Object {
		public:
			///INFO
			std::string name;
			virtual std::string ClassName() const { return "_NL::Core::Object"; };
			///STATES
			bool bactive = true; 
			bool bstatic = false; 
			///FUNC
			virtual void getInfo() {};
		};

		//PRIMITIVES

		struct Vertex {
			glm::vec3 P;		//Position
			glm::vec3 N;		//Normal
			glm::vec3 C;		//Color
			glm::vec2 T;		//TexCoordinates
		};

		struct Mesh {
			Vertex* verts;		//Triangles
			GLuint* indices;	//indices
		};

		struct transform {
			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;
		};
	}
}
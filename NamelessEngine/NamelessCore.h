#pragma once

///ExternalLibs
#include<GL\glew.h>
#include <glm\glm.hpp>
#include <iostream>
#include <vector>


namespace _NL{
	namespace Core {

		//======================
		/*CPPscript*/
		class CppScript 
		{
		public:
			virtual void Start() = 0;
			virtual void Update() = 0;
		};

		//======================
		/*All the Components that can be added to class: _NL::Object::GameObject*/
		class Component {
		public:
			virtual std::string ClassName() const = 0;
			bool bactive = true;
		};

		//======================
		/*All the Components that can be added to class: _NL::Core::Component*/
		class Element {
		public:
			///INFO
			std::string name;
			virtual std::string ClassName() const = 0;
		};


		//======================
		/*Object*/
		class Object {
		public:
			
			//======================
			//OBJECT PROPERTIES 
			
			std::string name;
			Object *Parent = 0;
			//std::vector<Object*> Childs;
			virtual std::string ClassName() const = 0;
			
			//======================
			//STATES
			
			bool bactive = true;
			bool bstatic = false;
			
			//======================
			//INFO
		
			virtual void getInfo() {};

			//======================
			//COMPONENTS

			int addComponent(_NL::Core::Component *C)
			{
				for each (_NL::Core::Component* c in Components)
				{
					if (c->ClassName() == "_NL::Component::Script") {
						//LET ADD MULTIPLE SCRIPTS
					}else if (c->ClassName() == C->ClassName()) {
						std::cout << "ERROR -1 :" << this->name.c_str() << " Object Component List Already Has a " << C->ClassName().c_str() << " Component." << std::endl;
						return -1;
					}
				}
				Components.push_back(C);
				std::cout << C->ClassName().c_str() << " Component Added to " << this->name.c_str() << std::endl;

				return 0;
			};

			template<typename T>
			T* getComponent()
			{
				T comp;
				for each (_NL::Core::Component* c in this->Components)
				{
					//std::cout << c->ClassName() << std::endl << comp.ClassName() << std::endl;
					if (c->ClassName() == comp.ClassName()) {
						return dynamic_cast<T*>(c);
					}
				}
				return NULL;
			}

			//template<typename T>
			//T* getScriptComponent()
			//{
			//	std::vector<_NL::Core::Component*> Ss;
			//	for each (_NL::Core::Component* c in this->Components)
			//	{
			//		//std::cout << c->ClassName() << std::endl;
			//		if (c->ClassName() == "_NL::Component::Script") {
			//			Ss.push_back(c);
			//		}
			//	}
			//	for each (_NL::Component::Script <_NL::Core::CppScript> * S in Ss)
			//	{
			//		if (typeid(S) == typeid(Ss)) {
			//			return dynamic_cast<T&>(*S);
			//		}
			//	}
			//};
		   
			std::vector<_NL::Core::Component*> Components;
		};

		//======================
		/*PRIMITIVES*/
		//======================

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
			glm::vec3 EulerRotation;
			glm::mat4 MatrixRotation;
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

		struct LightProperties {
			glm::vec4 lightColor = glm::vec4(1, 1, 1, 1);
			glm::vec3 lightPosition = glm::vec3(0, 0, 0);
			GLfloat PADDING0;
			GLfloat radiusOfInfluence = 1;
			glm::vec3 PADDING1;
			//glm::vec3 rotationAxis = glm::vec3(1, 0, 0);
			//GLfloat rotationAngle = 45.0f;
		};

	}
}
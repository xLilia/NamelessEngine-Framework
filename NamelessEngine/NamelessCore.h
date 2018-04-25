#pragma once
#define NL_PI 3.14159265359
///ExternalLibs
#include<GL\glew.h>
#include <glm\glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <iostream>
#include <vector>
#include "GLError.h"

namespace _NL{
	namespace Core {

		///ATRIBUTES
		const static GLuint Pos_atrib = 0;
		const static GLuint Norm_atrib = 1;
		const static GLuint Tangent_atrib = 2;
		const static GLuint TexC_atrib = 3;

		const static GLuint InstModelMatrix0_atrib = 4;
		//InstModelMatrix1_atrib = 5
		//InstModelMatrix2_atrib = 6
		//InstModelMatrix3_atrib = 7
		///UNIFORMS
		const static GLuint EyePos_uniform = 8;
		//const static GLuint ModelMatrix_uniform = 5; //Model
		const static GLuint ViewMatrix_uniform = 9; //View
		const static GLuint ProjectionMatrix_uniform = 10; //projection
		
		///TEXTURES[0-5]
		const static GLuint ALbedoTexture_uniform = 11;
		const static GLuint RoughnessTexture_uniform = 12;
		const static GLuint MetalnessTexture_uniform = 13;
		const static GLuint NormalTexture_uniform = 14;
		const static GLuint AmbientOculusionTexture_uniform = 15;
		const static GLuint AmbientIrradianceTexture_uniform = 16;
		const static GLuint PreFilterTexture_uniform = 17;
		const static GLuint BRDF2DLUTTexture_uniform = 18;

		//---------------------------------------------------------------------------------
		/*CPPscript*/
		class CppScript 
		{
		public:
			virtual void Start() = 0;
			virtual void Update() = 0;
		};

		//---------------------------------------------------------------------------------
		/*All the Components that can be added to class: _NL::Object::GameObject*/
		class Component {
		public:
			virtual std::string ClassName() const = 0;
			bool bactive = true;
		};

		//---------------------------------------------------------------------------------
		/*All the Components that can be added to class: _NL::Core::Component*/
		class Element {
		public:
			///INFO
			virtual std::string ClassName() const = 0;
		};

		class UI
		{
		public:
			virtual std::string ClassName() const = 0;
			glm::vec2 PositionRelativeToAnchor = glm::vec2(0,0);
			glm::vec2 AnchorPosition = glm::vec2(0,0);
			GLint Layer = 0;
			GLfloat RotationAngle;

			bool friend operator<(const UI& _this, const UI& other) {
				return _this.Layer < other.Layer;
			}
		};


		//---------------------------------------------------------------------------------
		/*Object*/
		class Object {
		public:
			
			//---------------------------------------------------------------------------------
			//OBJECT PROPERTIES 
			
			std::string name;
			Object *Parent = 0;
			//std::vector<Object*> Childs;
			virtual std::string ClassName() const = 0;
			
			//---------------------------------------------------------------------------------
			//STATES
			
			bool bactive = true;
			bool bstatic = false;
			
			//---------------------------------------------------------------------------------
			//INFO
		
			virtual void getInfo() {};

			//---------------------------------------------------------------------------------
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
				//std::cout << C->ClassName().c_str() << " Component Added to " << this->name.c_str() << std::endl;

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


		

		//---------------------------------------------------------------------------------
		/*PRIMITIVES*/
		//---------------------------------------------------------------------------------
		
		//MESH

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

		struct MaterialInstanceData {
			///PBR MODEL
			GLuint AlbedoTexId = NULL;
			GLuint RoughnessTexId = NULL;
			GLuint MetalnessTexId = NULL;
			GLuint NormalTexId = NULL;
			GLuint AmbientOculusionTexId = NULL;
			GLuint MTL_ID = NULL;
		};

		struct MeshData {
			std::vector<VertexPos> vPos;
			std::vector<VertexCol> vCol;
			std::vector<VertexNorm> vNorm;
			std::vector<VertexTexCoord> vTexC;
			std::vector<GLuint> MaterialStride;
		};

		//TRANSFORM



		struct transform {
			glm::quat QuaternionRotation = glm::quat(glm::vec3(0, 0, 0));
			glm::vec3 position = glm::vec3(0, 0, 0);
			glm::vec3 scale = glm::vec3(1, 1, 1);
		};


		//RENDER

		struct ScreenQuad {
			const GLfloat fullquad_v[8] =
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

		//struct ScreenQuadVAO {
		//	GLuint ID;
		//	ScreenQuadVAO() {
		//		GLuint vbo;
		//		glGenBuffers(1, &vbo);
		//		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*8, &ScreenQuad().fullquad_v[0], GL_STATIC_DRAW);
		//
		//		glGenVertexArrays(1, &ID);
		//		glEnableVertexArrayAttrib(ID, 0);
		//
		//		glVertexArrayAttribBinding(ID, 0, 0);
		//		glVertexArrayAttribFormat(ID, 0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 0);
		//		
		//	}
		//};
		

		
		inline void RenderQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h, bool removeProgramAferUse = true, GLuint Shader = 0, GLuint camId = 0) {
			if(Shader != 0) 
				glUseProgram(Shader);
			glViewport(x, y, w, h);
			
			//REPLACE WITH VAO
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			GLuint aScreenQuadTexCoords = 0;
			glEnableVertexAttribArray(aScreenQuadTexCoords);
			glVertexAttribPointer(aScreenQuadTexCoords, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)_NL::Core::ScreenQuad().fullquad_v);
			glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, (GLvoid*)_NL::Core::ScreenQuad().fullquad_i);
			glDisableVertexAttribArray(aScreenQuadTexCoords);
			
			if (removeProgramAferUse) 
				glUseProgram(0);
			check_gl_error();
		}

		struct LightProperties {
			glm::vec3 lightColor = glm::vec3(1, 1, 1);
			GLfloat PADDING0;
			glm::vec3 lightPosition = glm::vec3(0, 0, 0);
			GLfloat PADDING1;
			//glm::vec3 rotationAxis = glm::vec3(1, 0, 0);
			//GLfloat rotationAngle = 45.0f;
		};

	}
}
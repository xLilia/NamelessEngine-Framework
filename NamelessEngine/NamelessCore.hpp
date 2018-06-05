#pragma once

#include<GL\glew.h>
#include <glm\glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm\gtx\euler_angles.hpp>

//#include <PxPhysicsAPI.h>
#include <iostream>
#include <vector>
#include "GLError.h"

//namespace physx {
//	///NV_PHYSX 3.4
//	//https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/3.3.4/Manual/Startup.html
//
//	struct _NLPhysXFoundation {
//		static PxDefaultErrorCallback gDefaultErrorCallback;
//		static PxDefaultAllocator gDefaultAllocatorCallback;
//		static PxFoundation* mFoundation;
//		static void initialize() {
//			if (!mFoundation) {
//				mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
//				if (!mFoundation) printf("PxCreateFoundation failed!");
//			}
//		}
//	};
//
//}

/// ROOT of Nameless Engine API.
/// Use this namespace to access all other Namespaces and Classes in the Engine.
namespace _NL{
	/// Nameless Engine Core.
	/// Namespace containing Structs, Functions, Static values and Virtual Classes that make the 
	/// core of the engine.
	namespace Core {

		/// Value of PI accurate to the 12th decimal place:
		/// 3.14159265359
		#define NL_PI 3.14159265359

		/// GLSL Atributes and Uniforms, Default Shader Locations.
		/// Used to send data to specific binded Locations in this Program's Out of the box default shaders. */
		static const struct GLSL_AU {
			const static GLuint Pos_atrib = 0;									///< Vertex Position Atribute
			const static GLuint Norm_atrib = 1;									///< Vertex Normal Atribute
			const static GLuint Tangent_atrib = 2;								///< Vertex Tangent Atribute
			const static GLuint TexC_atrib = 3;									///< Vertex Texture Coordinates Atribute						
			const static GLuint InstModelMatrix0_atrib = 4;						///< Model Matrix Atribute
																				///< Binding points 4 to 7																	  
			const static GLuint EyePos_uniform = 8;								///< Camera Eye Position Uniform			
			const static GLuint ViewMatrix_uniform = 9; 						///< Camera View Matrix Uniform
			const static GLuint ProjectionMatrix_uniform = 10; 					///< Camera Projection Matrix Uniform
																				  
			const static GLuint ALbedoTexture_uniform = 11;						///< Albedo Texture Uniform
			const static GLuint RoughnessTexture_uniform = 12;					///< Roughness Texture Uniform
			const static GLuint MetalnessTexture_uniform = 13;					///< Metalness Texture Uniform
			const static GLuint NormalTexture_uniform = 14;						///< Normal Texture Uniform
			const static GLuint AmbientOculusionTexture_uniform = 15;			///< Ambient Oculusion Texture Uniform
			const static GLuint AmbientIrradianceTexture_uniform = 16;			///< Ambient Irradiance Texture Uniform
			const static GLuint PreFilterTexture_uniform = 17;					///< Pre-Filtered Texture Texture Uniform
			const static GLuint BRDF2DLUTTexture_uniform = 18;					///< BRDF 2D LUT Texture Texture Uniform
		};
		

		//NV_PHYSX 3.4
		//static physx::_NLPhysXFoundation PhysXFoundation;


		/// Virtual Class of _NL::Core::Component.
		/// Objects that inherit from this class can be added to a List of Components 
		/// belonging to Objects that inherit from the class "_NL::Object::GameObject".
		class Component {
		public:
			/// Every Sub Class must Overload this method.
			/// e.g : " return "_NL::Compoent::subgetTypeName" "
			virtual char* getTypeName() const = 0;
			/// Turn Component ON -> true |or| OFF -> false.
			/// Default value true.
			/// !!!Not Imlemented!!!
			bool bactive = true;
		};

		/// Virtual Class of _NL::Core::Element.
		/// Objects that inherit from this class are usually treated as singular entities
		/// that can be referenced by multiple components.
		class Element {
		public:
			/// Every Sub Class must Overload this method.
			/// e.g : " return "_NL::Element::subgetTypeName" "
			virtual char* getTypeName() const = 0;
		};

		/// Virtual Class of _NL::Core::Component.
		/// Objects that inherit from this class represent User Interface elements.
		class UI
		{
		public:
			/// Every Sub Class must Overload this method.
			/// e.g : " return "_NL::UI::subgetTypeName" "
			virtual char* getTypeName() const = 0;
			/// XY pixel coordinates on the screen for this UIelement.
			glm::vec2 AnchorPosition = glm::vec2(0, 0);
			/// XY pixel coordinates on the screen relative to AnchorPosition.
			/// Final Screen Position = [AnchorPosition + PositionRelativeToAnchor].
			glm::vec2 PositionRelativeToAnchor = glm::vec2(0,0);
			/// UI Element Layer value.
			/// Used to sort visibility between Overlapping UI Elements
			GLint Layer = 0;
			/// Rotation of UI Element.
			/// !!!Not Imlemented!!!
			GLfloat RotationAngle;
			/// Operator Overload < .
			/// Used to sort UI Elementes by Layers
			bool friend operator<(const UI& _this, const UI& other) {
				return _this.Layer < other.Layer;
			}
		};

		/// Virtual Class of _NL::Core::Object.
		/// Objects that inherit from this class can be added "_NL::Engine::WorldSpace"s scenes.
		class Object {
		public:
			/// Default Empty Constructor.
			/// Default Name : namelessObj
			Object() {
				this->name = "namelessObj";
			}
			/// Default Destructor.
			/// Clears stored Components
			virtual~Object() {
				for each(_NL::Core::Component* C in Components) {
					delete C;
				}
				this->Components.erase(Components.begin(), Components.end());
				this->Components.clear();
			}
			/// Object Name.
			/// Can be used for object Identification
			std::string name;
			/// Reference of Parent Object for this object.
			/// Parent Object Affects this Object's Transform.
			Object *Parent = 0;
			
			//Reference of Parent Object for this object.
			//Parent Object Affects this Object's Transform.
			//std::vector<Object*> Childs;

			/// Every Sub Class must Overload this method.
			/// e.g : " return "_NL::UI::subgetTypeName" "
			virtual char* getTypeName() const {
				return "_NL::Core::Object";
			};
			
			//---------------------------------------------------------------------------------
			//STATES
			
			//bool bactive = true;
			//bool bstatic = false;

			//---------------------------------------------------------------------------------
			//COMPONENTS

			int addComponent(_NL::Core::Component *C)
			{
				for each (_NL::Core::Component* c in Components)
				{
					if (c->getTypeName() == "_NL::Component::CppScript") {
						//LET ADD MULTIPLE SCRIPTS
					}else if (c->getTypeName() == C->getTypeName()) {
						std::cout << "ERROR -1 :" << this->name.c_str() << " Object Component List Already Has a " << C->getTypeName() << " Component." << std::endl;
						return -1;
					}
				}
				Components.push_back(C);
				//std::cout << C->getTypeName().c_str() << " Component Added to " << this->name.c_str() << std::endl;

				return 0;
			};

			template<typename ComponentType>
			ComponentType* getComponent()
			{
				ComponentType comp;
				for each (_NL::Core::Component* c in this->Components)
				{
					if (c->getTypeName() == comp.getTypeName()) {
						return dynamic_cast<ComponentType*>(c);
					}
				}
				return nullptr;
			}

			std::vector<_NL::Core::Component*> Components;
		};

		typedef std::vector<_NL::Core::Object*> ObjInstanceList;
		typedef std::vector<ObjInstanceList> ObjTypeList;
		typedef std::vector<ObjTypeList> ObjList;

		/*CoreScript*/
		class Script
		{
		public:
			_NL::Core::Object* _this;
			bool awake = false;
			virtual void Start() = 0 { awake = true; };
			virtual void Update() = 0;
			virtual void End() { awake = false; };
		};

		//---------------------------------------------------------------------------------
		/*PRIMITIVES*/
		//---------------------------------------------------------------------------------
		
		//MESH

		//struct VertexPos {
		//	glm::vec3 Pos;		//Position			
		//};
		//struct VertexNorm {
		//	glm::vec3 Norm;		//Normal
		//};
		//struct VertexCol {
		//	glm::vec3 Col;		//Color
		//};
		//struct VertexTexCoord {
		//	glm::vec2 TexCoord;	//TexCoordinates 
		//};
		//
		
		//struct vIndices {
		//	GLuint v[3];
		//	GLuint vt[3];
		//	GLuint vn[3];
		//	GLint MTL_ID;
		//};

		struct MaterialInstanceData {
			///PBR MODEL
			GLuint AlbedoTexId = NULL;
			GLuint RoughnessTexId = NULL;
			GLuint MetalnessTexId = NULL;
			GLuint NormalTexId = NULL;
			GLuint AmbientOculusionTexId = NULL;
			GLuint MTL_ID = NULL;
		};

		struct MeshInstanceData {
			std::vector<glm::vec3>mesh_positions_array;
			std::vector<glm::vec3>mesh_normals_array;
			std::vector<glm::vec2>mesh_map_array;
			std::vector<glm::vec3>mesh_triangles_array;
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
			glm::vec3 lightColor = glm::vec3(1,0,1);		
			GLfloat PADDING0 = 0;
																		 
			glm::vec3 lightPosition = glm::vec3(0.0f);
			GLfloat PADDING1 = 0;
									 
			glm::vec3 lightDirection = glm::vec3(0.0f);
			GLfloat PADDING2 = 0;

			GLfloat lightSpotInnerAngle = 0.0f;
			GLfloat lightSpotOuterAngle = 0.0f;
			GLfloat PADDING3[2] = {0,0}; 

			void setPointLightProperties(glm::vec3 lightColor, glm::vec3 lightPosition) {
				this->lightColor = lightColor;
				this->lightPosition = lightPosition;
				this->lightDirection = glm::vec3(0.0f);
				this->lightSpotInnerAngle = 0.0f;
				this->lightSpotOuterAngle = 0.0f;
			}

			void setDirectionalLightProperties(glm::vec3 lightColor, glm::vec3 lightPosition, glm::vec3 lightDirection) {
				setPointLightProperties(lightColor, lightPosition);
				this->lightDirection = lightDirection;
				this->lightSpotInnerAngle = 0.0f;
				this->lightSpotOuterAngle = 0.0f;
			}

			void setSpotLightProperties(glm::vec3 lightColor, glm::vec3 lightPosition, glm::vec3 lightDirection, GLfloat InnerAngle, GLfloat OuterAngle) {
				setDirectionalLightProperties(lightColor, lightPosition, lightDirection);
				this->lightSpotInnerAngle = glm::cos(glm::radians(InnerAngle));
				this->lightSpotOuterAngle = glm::cos(glm::radians(OuterAngle));
			}
		};
		
		///WIP
		struct AnimationBone {
			std::vector<glm::mat4> TRANSFORM_Frame;
			std::vector<AnimationBone> Childs;
			AnimationBone* Parent = NULL;
			void addChild(AnimationBone& AB) {
				Childs.push_back(AB);
				AB.Parent = this;
			}
		};

	}
}
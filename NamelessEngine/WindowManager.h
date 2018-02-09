#pragma once
//#include"NamelessCore.h"
//#include "WorldSpace.h"
//#include"GameObject.h"
//#include"CameraObj.h"
#include "NL.hpp"
#include "GLError.h"
#include <SFML\Graphics.hpp>
#include <GL\glew.h>

namespace _NL {
	namespace Engine {
		class WindowManager
		{
		public:
			WindowManager(const char * WindowName, int Width, int height, bool fullscreen = false);
			void RunGameLoop();
			void OpenGLStart();
			void Start();
			void update();
			~WindowManager();
			sf::RenderWindow* window;
			_NL::Object::WorldSpace* CurrentScene;
		private:
			std::vector<_NL::Core::MeshData*> MeshDataStack;
			std::vector<std::vector<_NL::Core::vIndices>*> MeshElementsStack;
			GLuint vbo; //Vertex Buffer Object
			GLuint ebo;	//Element Buffer Object
			GLuint vao; //Vertex Array Object
			std::vector<GLfloat> VertsBuf;
			std::vector<GLuint> IndicesBuf;
			GLuint Pos_atrib = 0;
			GLuint Col_atrib = 1;
			GLuint Norm_atrib = 2;
			GLuint TexC_atrib = 3;
		};
	}
}



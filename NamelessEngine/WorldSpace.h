#pragma once
#include"NamelessCore.h"
#include"GameObject.h"
#include"Transform.h"
#include"CameraObj.h"
#include"SkyboxObj.h"

#include <vector>;

namespace _NL {
	namespace Engine {
		class WorldSpace
		{
		public:
			WorldSpace();
			~WorldSpace();
			
			void Instantiate(_NL::Object::GameObject *original, glm::vec3 position, glm::vec3 rotation);
			
			void addObjectToWorld(_NL::Core::Object *G);
			void showObjectList();
			_NL::Object::SkyboxObj* Skybox = NULL;
			std::vector<std::vector<_NL::Core::Object*>> ObjectList;
		
		};
	}
}
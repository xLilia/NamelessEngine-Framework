#pragma once
#include"NamelessCore.h"
#include "GameObject.h"
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
			
			_NL::Core::Object* Instantiate(_NL::Core::Object *original, glm::vec3 position, glm::quat quaternionRotation, glm::vec3 scale);
			void KillObjectInstance(_NL::Core::Object *object);
			void addObjectToWorld(_NL::Core::Object *G);
			void showObjectList();
			_NL::Object::SkyboxObj* Skybox = NULL;
			std::vector<std::vector<_NL::Core::Object*>> ObjectList;
		};
	}
}
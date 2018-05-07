#pragma once
#include"NamelessCore.hpp"
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
			template<typename ObjType>
			void KillObjectInstance(ObjType* object)
			{
				for (int o = 0; o < ObjectList.size(); o++) {
					for (int i = 0; i < ObjectList[o].size(); i++) {
						if (ObjectList[o][i] == object) {
							ObjectList[o].erase(ObjectList[o].begin() + i);
							return;
						}
					}
				}
			};

			void addObjectToWorld(_NL::Core::Object *G);
			void showObjectList();
			_NL::Object::SkyboxObj* Skybox = NULL;
			std::vector<std::vector<_NL::Core::Object*>> ObjectList;
		};
	}
}
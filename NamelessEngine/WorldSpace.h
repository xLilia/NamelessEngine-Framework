#pragma once
#include"NamelessCore.h"
#include"GameObject.h"
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

			void addObjectToWorld(_NL::Core::Object *G);
			void showObjectList();
			_NL::Object::SkyboxObj* Skybox = NULL;
			std::vector<_NL::Core::Object*> GetObjectList();
		private:
			std::vector<_NL::Core::Object*> ObjectList;
		};
	}
}
#pragma once
#include"NamelessCore.h"
#include"GameObject.h"
#include"CameraObj.h"
#include <vector>;

namespace _NL {
	namespace Object {
		class WorldSpace
		{
		public:
			WorldSpace();
			~WorldSpace();

			void addObjectToWorld(_NL::Core::Object *G);
			void showObjectList();
			std::vector<_NL::Core::Object*> GetObjectList();
			_NL::Object::CameraObj* MainCamera;
		private:
			
			std::vector<_NL::Core::Object*> ObjectList;
		};
	}
}
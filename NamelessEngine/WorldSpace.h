#pragma once
#include"NamelessCore.h"
#include"GameObject.h"

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

		private:
			std::vector<_NL::Core::Object*> ObjectList;
		};
	}
}
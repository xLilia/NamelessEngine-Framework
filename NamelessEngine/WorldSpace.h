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

			_NL::Core::ObjTypeList* getObjTypeListLocation(_NL::Core::Object * G);
			_NL::Core::ObjTypeList* getObjTypeListLocation(std::string ObjType);
			_NL::Core::ObjInstanceList* getObjInstanceListLocation(_NL::Core::Object * G, _NL::Core::ObjTypeList* T = nullptr);
			_NL::Core::ObjTypeList * addObjTypeListLocation(_NL::Core::Object * G);
			_NL::Core::ObjTypeList * addObjTypeListLocation(std::string ObjType);
			_NL::Core::ObjInstanceList * addObjInstanceListLocation(_NL::Core::Object * G, _NL::Core::ObjTypeList * T = nullptr);
			
			template<class CastToObjType>
			std::vector<CastToObjType*> getAllObjsOfType()
			{
				std::string ObjType = CastToObjType().getTypeName();
				std::vector<CastToObjType*> O;
				_NL::Core::ObjTypeList* OTL = getObjTypeListLocation(ObjType);
				if (OTL == nullptr) OTL = addObjTypeListLocation(ObjType);
				for (_NL::Core::ObjInstanceList& OIL : *OTL)
				for (_NL::Core::Object* o : OIL)
				{
					O.push_back(dynamic_cast<CastToObjType*>(o));
				}
				return O;
			}

			template<class ObjType>
			void KillObjectInstance(ObjType* Target)
			{
				if (Target == nullptr) return;
				_NL::Core::ObjTypeList* OTL = getObjTypeListLocation(Target);
				if (OTL == nullptr) OTL = addObjTypeListLocation(Target);
				GLuint Loc0 = -1;
				for (_NL::Core::ObjInstanceList& OIL : *OTL) {
					Loc0++;
					GLuint Loc1 = -1;
					for (_NL::Core::Object* o : OIL)
					{
						Loc1++;
						if (o == Target) {
							OIL.erase(OIL.begin() + Loc1);
							if (OIL.size() == 0) {
								OTL->erase(OTL->begin() + Loc0);
							}
							return;
						}
					}
				}
			}

			template<class CastToObjType, typename ObjType>
			CastToObjType* Instantiate(ObjType* original)
			{
				CastToObjType* CopyObj = new CastToObjType(*original);
				addObjectToWorld(CopyObj);
				return CopyObj;
			}

			_NL::Core::ObjInstanceList* addObjectToWorld(_NL::Core::Object *G);
			
			_NL::Object::SkyboxObj* Skybox = NULL;

			std::vector<std::string> ObjTypes;
			_NL::Core::ObjList ObjectLists;
			
			//std::vector<GLuint> Cameras;
			//std::vector<GLuint> Lights;
			//std::vector<GLuint> ParticleSystems;
			//std::vector<GLuint> UICanvas;
		};
	}
}
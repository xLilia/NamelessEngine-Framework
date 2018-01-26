#pragma once
#include <vector>;
#include "NamelessCore.h"
#include "GameObject.h"

class WorldSpace
{
public:
	WorldSpace();
	void addGameObjectToWorld(GameObject &G);
	std::vector<GameObject> GameObjectList;
	~WorldSpace();
};


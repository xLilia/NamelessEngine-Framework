#include "WorldSpace.h"

WorldSpace::WorldSpace()
{
	
}

void WorldSpace::addGameObjectToWorld(GameObject & G)
{
	GameObjectList.push_back(G);
}


WorldSpace::~WorldSpace()
{
}

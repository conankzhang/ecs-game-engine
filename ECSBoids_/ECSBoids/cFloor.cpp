// Includes
//=========

#include "cFloor.h"

#include "cRenderComponent.h"
#include <Engine/EntityComponentSystem/ComponentManager.h>

// Initialization / Clean Up
//--------------------------

eae6320::cFloor::cFloor()
{
}

void eae6320::cFloor::Initialize()
{
	AddComponent<cRenderComponent>("data/prefabs/floor.prefab", Math::sVector(), Math::cQuaternion());
}

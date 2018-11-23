// Includes
//=========

#include "cBoid.h"

#include "cRenderComponent.h"
#include <Engine/EntityComponentSystem/ComponentManager.h>

// Initialization / Clean Up
//--------------------------

eae6320::cBoid::cBoid()
{

}

void eae6320::cBoid::Initialize()
{
	AddComponent<cRenderComponent>("data/prefabs/boid.prefab", Math::sVector(), Math::cQuaternion());
}

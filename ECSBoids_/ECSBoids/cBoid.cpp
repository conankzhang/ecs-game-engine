// Includes
//=========

#include "cBoid.h"

#include "cRenderComponent.h"
#include "cBoidComponent.h"

#include <Engine/EntityComponentSystem/ComponentManager.h>

// Initialization / Clean Up
//--------------------------

eae6320::cBoid::cBoid()
{

}

void eae6320::cBoid::Initialize()
{

	AddComponent<cRenderComponent>("data/prefabs/boid.prefab", Math::sVector(0.0f, 2.5f, 0.0f), Math::cQuaternion());
	AddComponent<cBoidComponent>(GetComponent<cRenderComponent>());
}

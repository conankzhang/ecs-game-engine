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

	AddComponent<cRenderComponent>("data/prefabs/boid.prefab", Math::sVector((float)(rand() % 5), 5.0f, (float)(rand() % 5)), Math::cQuaternion());
	GetComponent<cRenderComponent>()->m_clampSpeed = true;
	AddComponent<cBoidComponent>(GetComponent<cRenderComponent>());
}

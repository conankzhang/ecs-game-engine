// Includes
//=========

#include "cGoal.h"

#include "cRenderComponent.h"
#include <Engine/EntityComponentSystem/ComponentManager.h>

// Initialization / Clean Up
//--------------------------

eae6320::cGoal::cGoal()
{

}

void eae6320::cGoal::Initialize()
{
	AddComponent<cRenderComponent>("data/prefabs/goal.prefab", Math::sVector(), Math::cQuaternion());
}

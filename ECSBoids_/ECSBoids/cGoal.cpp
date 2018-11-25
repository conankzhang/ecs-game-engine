// Includes
//=========

#include "cGoal.h"

#include "cRenderComponent.h"
#include "cGoalComponent.h"
#include <Engine/EntityComponentSystem/ComponentManager.h>

// Initialization / Clean Up
//--------------------------

eae6320::cGoal::cGoal(cCameraComponent* i_cameraToFollow)
{
	m_cameraToFollow = i_cameraToFollow;
	m_rotationSpeed = 6.0f;
}

void eae6320::cGoal::Initialize()
{
	AddComponent<cRenderComponent>("data/prefabs/goal.prefab", Math::sVector(0.0f, 2.5f, 0.0f), Math::cQuaternion());
	AddComponent<cGoalComponent>(GetComponent<cRenderComponent>());
	GetComponent<cRenderComponent>()->SetAngularSpeed(m_rotationSpeed);
	GetComponent<cRenderComponent>()->SetRotationAxis(Math::sVector(0.0f, 0.0f, 1.0f));
}

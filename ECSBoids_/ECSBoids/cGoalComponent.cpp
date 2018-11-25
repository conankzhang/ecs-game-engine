// Includes
//=========

#include "cGoalComponent.h"
#include "cCameraComponent.h"
#include "cRenderComponent.h"

// Initialization / Clean Up
//--------------------------

eae6320::cGoalComponent::cGoalComponent(cRenderComponent* i_goalRenderComponent)
{
	m_goalRenderComponent = i_goalRenderComponent;
}

// Implementation
//===============

eae6320::Math::sVector eae6320::cGoalComponent::GetPosition()
{
	return m_goalRenderComponent->GetPosition();
}

void eae6320::cGoalComponent::SetPosition(Math::sVector i_position)
{
	m_goalRenderComponent->SetPosition(i_position);
}

void eae6320::cGoalComponent::SetVelocity(Math::sVector i_velocity)
{
	m_goalRenderComponent->SetVelocity(i_velocity);
}

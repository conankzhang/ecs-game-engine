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

eae6320::Math::sVector eae6320::cGoalComponent::GetPosition()
{
	return m_goalRenderComponent->GetPosition();
}

void eae6320::cGoalComponent::SetPosition(Math::sVector i_desiredPosition)
{
	m_goalRenderComponent->SetPosition(i_desiredPosition);
}

// Includes
//=========

#include "cBoidComponent.h"
#include "cRenderComponent.h"

// Initialization / Clean Up
//--------------------------

eae6320::cBoidComponent::cBoidComponent(cRenderComponent* i_boidRenderComponent)
{
	m_boidRenderComponent = i_boidRenderComponent;
}

// Implementation
//===============

eae6320::Math::sVector eae6320::cBoidComponent::GetPosition()
{
	return m_boidRenderComponent->GetPosition();

}

void eae6320::cBoidComponent::SetVelocity(Math::sVector i_velocity)
{
	m_boidRenderComponent->SetVelocity(i_velocity);

	Math::sVector forward = m_boidRenderComponent->GetForward();
	float exteriorProduct = i_velocity.x * forward.z - i_velocity.z * forward.x;

	m_boidRenderComponent->SetAngularSpeed(exteriorProduct);
}

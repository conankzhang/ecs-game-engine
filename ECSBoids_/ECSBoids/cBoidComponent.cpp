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

eae6320::Math::sVector eae6320::cBoidComponent::GetForward()
{
	return m_boidRenderComponent->GetForward();
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
}

void eae6320::cBoidComponent::SetAcceleration(Math::sVector i_acceleration)
{
	m_boidRenderComponent->SetAcceleration(i_acceleration);
}

void eae6320::cBoidComponent::SetAngularSpeedBasedOnPosition(Math::sVector i_position)
{
	Math::sVector forward = m_boidRenderComponent->GetForward();
	float exteriorProduct = i_position.x * forward.z - i_position.z * forward.x;

	m_boidRenderComponent->SetAngularSpeed(exteriorProduct);
}

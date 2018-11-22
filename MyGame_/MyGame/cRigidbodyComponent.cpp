/*
	Rigidbody Component implementation.
*/

// Includes
//=========

#include "cRigidbodyComponent.h"

#include <Engine/Graphics/cEffect.h>
#include <Engine/Platform/Platform.h>
#include <Engine/Math/cMatrix_transformation.h>

// Class Declaration
//==================

eae6320::cRigidbodyComponent::cRigidbodyComponent(eae6320::Math::sVector i_position, eae6320::Math::cQuaternion i_orientation)
{
	m_rigidBody.position = i_position;
	m_rigidBody.orientation = i_orientation;
}

eae6320::cRigidbodyComponent::~cRigidbodyComponent()
{

}

void eae6320::cRigidbodyComponent::Update(const float i_elapsedSecondCount_sinceLastUpdate)
{
	m_rigidBody.Update(i_elapsedSecondCount_sinceLastUpdate);
}

void eae6320::cRigidbodyComponent::SetVelocity(eae6320::Math::sVector i_velocity)
{
	m_rigidBody.velocity = i_velocity;
}

eae6320::Math::cMatrix_transformation eae6320::cRigidbodyComponent::GetTransform(const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	return eae6320::Math::cMatrix_transformation(m_rigidBody.PredictFutureOrientation(i_elapsedSecondCount_sinceLastSimulationUpdate), m_rigidBody.PredictFuturePosition(i_elapsedSecondCount_sinceLastSimulationUpdate));
}


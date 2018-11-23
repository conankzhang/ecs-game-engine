// Includes
//=========

#include "cCameraComponent.h"

#include <Engine/Math/cMatrix_transformation.h>

// Initialization / Clean Up
//--------------------------

eae6320::cCameraComponent::cCameraComponent(Math::sVector i_position, Math::cQuaternion i_orientation)
{
	m_rigidBody.position = i_position;
	m_rigidBody.orientation = i_orientation;
}

void eae6320::cCameraComponent::Update(const float i_deltaTime)
{
	m_rigidBody.Update(i_deltaTime);
}

void eae6320::cCameraComponent::SetVelocity(Math::sVector i_velocity)
{
	m_rigidBody.velocity = i_velocity;
}

eae6320::Math::cMatrix_transformation eae6320::cCameraComponent::GetWorldToCameraTransform(const float i_deltaTime)
{
	return Math::cMatrix_transformation::CreateWorldToCameraTransform(Math::cMatrix_transformation(m_rigidBody.PredictFutureOrientation(i_deltaTime), m_rigidBody.PredictFuturePosition(i_deltaTime)));
}

eae6320::Math::cMatrix_transformation eae6320::cCameraComponent::GetCameraToProjectedTransform()
{
	return Math::cMatrix_transformation::CreateCameraToProjectedTransform_perspective(m_verticalFieldOfView_inRadians, m_aspectRatio, m_z_nearPlane, m_z_farPlane);

}

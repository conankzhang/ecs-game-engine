// Includes
//=========

#include "cCameraComponent.h"

#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/ControllerInput/ControllerInput.h>

// Initialization / Clean Up
//--------------------------

eae6320::cCameraComponent::cCameraComponent(Math::sVector i_position, Math::cQuaternion i_orientation)
{
	m_rigidBody.position = i_position;
	m_rigidBody.orientation = i_orientation;
}

// Implementation
//===============

void eae6320::cCameraComponent::Update(const float i_deltaTime)
{
	m_rigidBody.Update(i_deltaTime);
}

void eae6320::cCameraComponent::SetVelocity(Math::sVector i_velocity)
{
	m_rigidBody.velocity = i_velocity;
}

eae6320::Math::sVector eae6320::cCameraComponent::GetForward()
{
	return m_rigidBody.orientation.CalculateForwardDirection();
}

eae6320::Math::sVector eae6320::cCameraComponent::GetPosition()
{
	return m_rigidBody.position;
}

eae6320::Math::sVector eae6320::cCameraComponent::GetPositionInFrontOfCamera(float i_distanceInFront)
{
	Math::sVector position = GetPosition();

	Math::sVector forward = GetForward();
	forward.Normalize();

	return position += forward * i_distanceInFront;
}

eae6320::Math::cMatrix_transformation eae6320::cCameraComponent::GetWorldToCameraTransform(const float i_deltaTime)
{
	return Math::cMatrix_transformation::CreateWorldToCameraTransform(Math::cMatrix_transformation(m_rigidBody.PredictFutureOrientation(i_deltaTime), m_rigidBody.PredictFuturePosition(i_deltaTime)));
}

eae6320::Math::cMatrix_transformation eae6320::cCameraComponent::GetCameraToProjectedTransform()
{
	return Math::cMatrix_transformation::CreateCameraToProjectedTransform_perspective(m_verticalFieldOfView_inRadians, m_aspectRatio, m_z_nearPlane, m_z_farPlane);

}

void eae6320::cCameraComponent::UpdateInput()
{
	HandleMovement();
	HandleRotation();
}

void eae6320::cCameraComponent::HandleMovement()
{
	Math::sVector leftStickDeflection = UserInput::ControllerInput::GetNormalizedStickDeflection(UserInput::ControllerInput::ControllerKeyCodes::LEFT_STICK, 0);
	float leftTriggerDeflection = -UserInput::ControllerInput::GetNormalizedTriggerDeflection(UserInput::ControllerInput::ControllerKeyCodes::LEFT_TRIGGER, 0);
	float rightTriggerDeflection = UserInput::ControllerInput::GetNormalizedTriggerDeflection(UserInput::ControllerInput::ControllerKeyCodes::RIGHT_TRIGGER, 0);

	leftStickDeflection *= m_movementSpeed;
	leftTriggerDeflection *= m_movementSpeed;
	rightTriggerDeflection *= m_movementSpeed;

	Math::sVector forward = GetForward() * leftStickDeflection.y;

	Math::sVector lateral = Math::Cross(GetForward(), Math::sVector(0.0f, 1.0f, 0.0f));
	lateral *= leftStickDeflection.x;

	Math::sVector vertical = Math::sVector(0.0f, 1.0f, 0.0f);
	vertical *= leftTriggerDeflection + rightTriggerDeflection;

	SetVelocity(forward + lateral + vertical);
}

void eae6320::cCameraComponent::HandleRotation()
{
	Math::sVector rightStickDeflection = UserInput::ControllerInput::GetNormalizedStickDeflection(UserInput::ControllerInput::ControllerKeyCodes::RIGHT_STICK, 0);
	m_rigidBody.angularSpeed = -rightStickDeflection.x * m_rotationSpeed;
}

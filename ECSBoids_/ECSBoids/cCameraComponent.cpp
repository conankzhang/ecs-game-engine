// Includes
//=========

#include "cCameraComponent.h"

#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/ControllerInput/ControllerInput.h>
#include <Engine/UserInput/UserInput.h>

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
	if (m_rigidBody.position.y < 1.0f && i_velocity.y < 0.0f)
	{
		m_rigidBody.velocity = Math::sVector(i_velocity.x, 0.0f, i_velocity.z);
		return;
	}
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
	HandleMovementInput();
	HandleRotationInput();
}

void eae6320::cCameraComponent::HandleMovementInput()
{
	Math::sVector movement = GetControllerMovementInput();
	movement += GetKeyboardMovementInput();
	SetVelocity(movement);
}

eae6320::Math::sVector eae6320::cCameraComponent::GetControllerMovementInput()
{
	float x_movement = 0.0f;
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::A))
	{
		x_movement -= 1.0f;
	}

	if (UserInput::IsKeyPressed(UserInput::KeyCodes::D))
	{
		x_movement += 1.0f;
	}

	float y_movement = 0.0f;
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Up))
	{
		y_movement += 1.0f;
	}

	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Down))
	{
		y_movement -= 1.0f;
	}

	float z_movement = 0.0f;
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::W))
	{
		z_movement += 1.0f;
	}

	if (UserInput::IsKeyPressed(UserInput::KeyCodes::S))
	{
		z_movement -= 1.0f;
	}

	Math::sVector forward = GetForward() * z_movement;

	Math::sVector lateral = Math::Cross(GetForward(), Math::sVector(0.0f, 1.0f, 0.0f));
	lateral *= x_movement;

	Math::sVector vertical = Math::sVector(0.0f, 1.0f, 0.0f);
	vertical *= y_movement;

	return (forward + lateral + vertical) * m_movementSpeed;
}

eae6320::Math::sVector eae6320::cCameraComponent::GetKeyboardMovementInput()
{
	Math::sVector leftStickDeflection = UserInput::ControllerInput::GetNormalizedStickDeflection(UserInput::ControllerInput::ControllerKeyCodes::LEFT_STICK, 0);
	float leftTriggerDeflection = -UserInput::ControllerInput::GetNormalizedTriggerDeflection(UserInput::ControllerInput::ControllerKeyCodes::LEFT_TRIGGER, 0);
	float rightTriggerDeflection = UserInput::ControllerInput::GetNormalizedTriggerDeflection(UserInput::ControllerInput::ControllerKeyCodes::RIGHT_TRIGGER, 0);

	Math::sVector forward = GetForward() * leftStickDeflection.y;

	Math::sVector lateral = Math::Cross(GetForward(), Math::sVector(0.0f, 1.0f, 0.0f));
	lateral *= leftStickDeflection.x;

	Math::sVector vertical = Math::sVector(0.0f, 1.0f, 0.0f);
	vertical *= leftTriggerDeflection + rightTriggerDeflection;

	return (forward + lateral + vertical) * m_movementSpeed;
}

void eae6320::cCameraComponent::HandleRotationInput()
{
	float angularSpeed = GetControllerRotationInput();
	angularSpeed += GetKeyboardRotationInput();
	m_rigidBody.angularSpeed = angularSpeed;
}

float eae6320::cCameraComponent::GetControllerRotationInput()
{
	Math::sVector rightStickDeflection = UserInput::ControllerInput::GetNormalizedStickDeflection(UserInput::ControllerInput::ControllerKeyCodes::RIGHT_STICK, 0);
	return -rightStickDeflection.x * m_rotationSpeed;
}

float eae6320::cCameraComponent::GetKeyboardRotationInput()
{
	float angularSpeed = 0.0f;
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Left))
	{
		angularSpeed += 1.0f;
	}

	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Right))
	{
		angularSpeed -= 1.0f;
	}
	return angularSpeed * m_rotationSpeed;
}

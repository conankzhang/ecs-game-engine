/*
	Camera Component interface.
*/

#ifndef CCAMERACOMPONENT_H
#define CCAMERACOMPONENT_H

// Includes
//=========
#include <Engine/EntityComponentSystem/Component.h>
#include <Engine/Physics/sRigidBodyState.h>

// Forward Declarations
//=====================

// Class Declaration
//==================
namespace eae6320
{
	class cCameraComponent : public ECS::Component<cCameraComponent>
	{
	public:
		cCameraComponent(Math::sVector i_position, Math::cQuaternion i_orientation);

		void Update(const float i_deltaTime);
		void SetVelocity(Math::sVector i_velocity);
		Math::sVector GetForward();
		Math::sVector GetPosition();
		Math::sVector GetPositionInFrontOfCamera(float i_distanceInFront);

		Math::cMatrix_transformation GetWorldToCameraTransform(const float i_deltaTime);
		Math::cMatrix_transformation GetCameraToProjectedTransform();

		void UpdateInput();

	protected:
		void HandleMovementInput();
		Math::sVector GetControllerMovementInput();
		Math::sVector GetKeyboardMovementInput();
		Math::sVector GetMovementInput(float i_xInput, float i_yInput, float i_zInput);

		void HandleRotationInput();
		float GetControllerRotationInput();
		float GetKeyboardRotationInput();

	protected:
		Physics::sRigidBodyState m_rigidBody;

	private:
		const float m_verticalFieldOfView_inRadians = 45.0f;
		const float m_aspectRatio = 1.777f;
		const float m_z_nearPlane = 0.1f;
		const float m_z_farPlane = 500.0f;

		const float m_verticalMovementSpeed = 10.0f;
		const float m_horizontalMovementSpeed = 1500.0f;
		const float m_rotationSpeed = 20000.0f;
	};
}

#endif // CCAMERACOMPONENT_H

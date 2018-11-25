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
		void HandleMovement();
		void HandleRotation();

	protected:
		Physics::sRigidBodyState m_rigidBody;

	private:
		const float m_verticalFieldOfView_inRadians = 45.0f;
		const float m_aspectRatio = 1.777f;
		const float m_z_nearPlane = 0.1f;
		const float m_z_farPlane = 100.0f;

		const float m_movementSpeed = 5.0f;
		const float m_rotationSpeed = 2.0f;
	};
}

#endif // CCAMERACOMPONENT_H

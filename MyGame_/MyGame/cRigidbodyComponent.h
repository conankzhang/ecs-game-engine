/*
	Rigidbody Component interface.
*/

#ifndef CRIGIDBODYCOMPONENT_H
#define CRIGIDBODYCOMPONENT_H

// Includes
//=========
#include <Engine/Physics/sRigidBodyState.h>
#include <Engine/EntityComponentSystem/ECS.h>

// Forward Declarations
//=====================

namespace eae6320
{
	namespace Graphics
	{
		class cEffect;
	}
}

// Class Declaration
//==================
namespace eae6320
{
	class cRigidbodyComponent : public eae6320::ECS::Component<cRigidbodyComponent>
	{
	public:
		cRigidbodyComponent(eae6320::Math::sVector i_position, eae6320::Math::cQuaternion i_orientation);
		~cRigidbodyComponent();
		
		void Update(const float i_elapsedSecondCount_sinceLastUpdate);
		void SetVelocity(eae6320::Math::sVector i_velocity);
		eae6320::Math::cMatrix_transformation GetTransform(const float i_elapsedSecondCount_sinceLastSimulationUpdate);

	protected:
		eae6320::Physics::sRigidBodyState m_rigidBody;
	};
}

#endif // RIGIDBODYCOMPONENT_H

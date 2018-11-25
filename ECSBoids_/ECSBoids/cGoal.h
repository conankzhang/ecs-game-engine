/*
	Goal interface.
*/

#ifndef CGOAL_H
#define CGOAL_H

// Includes
//=========
#include <Engine/EntityComponentSystem/Entity.h>

// Forward Declarations
//=====================
namespace eae6320
{
	class cCameraComponent;
}

// Class Declaration
//==================

namespace eae6320
{
	class cGoal: public ECS::Entity<cGoal>
	{
	public:
		cGoal(cCameraComponent* i_cameraToFollow);

		virtual void Initialize() override;

	private:
		cCameraComponent* m_cameraToFollow;
		float m_rotationSpeed;
	};
}

#endif	// CGOAL_H

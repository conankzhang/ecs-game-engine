/*
	Goal Component interface.
*/

#ifndef CGOALCOMPONENT_H
#define CGOALCOMPONENT_H

// Includes
//=========
#include <Engine/EntityComponentSystem/Component.h>
#include <Engine/Math/sVector.h>

// Forward Declarations
//=====================
namespace eae6320
{
	class cCameraComponent;
	class cRenderComponent;
}

// Class Declaration
//==================
namespace eae6320
{
	class cGoalComponent : public ECS::Component<cGoalComponent>
	{
	public:
		cGoalComponent(cRenderComponent* i_goalRenderComponent);

		Math::sVector GetPosition();
		void SetPosition(Math::sVector i_position);
		void SetVelocity(Math::sVector i_velocity);
	protected:
		cRenderComponent* m_goalRenderComponent;
	};
}

#endif // CGOALCOMPONENT_H

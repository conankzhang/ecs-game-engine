/*
	Boid Component interface.
*/

#ifndef CBOIDCOMPONENT_H
#define CBOIDCOMPONENT_H

// Includes
//=========
#include <Engine/EntityComponentSystem/Component.h>
#include <Engine/Math/sVector.h>

// Forward Declarations
//=====================
namespace eae6320
{
	class cRenderComponent;
}

// Class Declaration
//==================
namespace eae6320
{
	class cBoidComponent : public ECS::Component<cBoidComponent>
	{
	public:
		cBoidComponent(cRenderComponent* i_boidRenderComponent);

		Math::sVector GetPosition();
		void SetVelocity(Math::sVector i_velocity);

	protected:
		cRenderComponent* m_boidRenderComponent;
	};
}

#endif // CBOIDCOMPONENT_H

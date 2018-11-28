/*
	Boid System interface.
*/

#ifndef CBOIDSYSTEM_H
#define CBOIDSYSTEM_H

// Includes
//=========
#include <Engine/EntityComponentSystem/System.h>
#include <Engine/Math/cQuaternion.h>

// Forward Declarations
//=====================
namespace eae6320
{
	class cGoalComponent;
	class cBoidComponent;
}

// Class Declaration
//==================
namespace eae6320
{
	class cBoidSystem : public ECS::System<cBoidSystem>
	{
	public:

		cBoidSystem();
		~cBoidSystem();

		virtual void Initialize() override;
		virtual void Update(float i_deltaTime) override;

	protected:

		Math::sVector CalculateSeparation(cBoidComponent* i_boidComponent);
		Math::sVector CalculateCohesion(cBoidComponent* i_boidComponent);

	private:
		cGoalComponent* m_goal;
		float m_seperationDistance;
		float m_cohesionDistance;
	};
}

#endif // CINPUTSYSTEM_H

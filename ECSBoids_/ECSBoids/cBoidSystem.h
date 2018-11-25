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
		Math::cQuaternion GetOrientationFromVector(Math::sVector i_direction);

	private:
		cGoalComponent* m_goal;
	};
}

#endif // CINPUTSYSTEM_H

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
		virtual void PreUpdate(float i_deltaTime) override;
		virtual void Update(float i_deltaTime) override;

	protected:

		void CalculateNeighbors(std::vector<cBoidComponent*>& o_neighbors, cBoidComponent* i_boidComponent);
		Math::sVector CalculateSeparation(const std::vector<cBoidComponent*>& i_neighbors, cBoidComponent* i_boidComponent);
		Math::sVector CalculateCohesion(const std::vector<cBoidComponent*>& i_neighbors, cBoidComponent* i_boidComponent);

	private:
		cGoalComponent* m_goal;
		float m_neighborDistance;
		float m_seperationDistance;
		float m_cohesionDistance;

		float m_seperationStrength;
		float m_cohesionStrength;
		float m_goalStrength;

		float m_changeSpeed;
	};
}

#endif // CINPUTSYSTEM_H

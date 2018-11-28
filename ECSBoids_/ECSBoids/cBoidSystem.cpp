// Includes
//=========

#include "cBoidSystem.h"
#include "cBoidComponent.h"
#include "cGoalComponent.h"

// Initialization / Clean Up
//--------------------------

eae6320::cBoidSystem::cBoidSystem()
{

}

eae6320::cBoidSystem::~cBoidSystem()
{

}

void eae6320::cBoidSystem::Initialize()
{
	auto goalIterator = m_componentManager->begin<cGoalComponent>();
	m_goal = dynamic_cast<cGoalComponent*>(goalIterator->second);

	m_seperationDistance = 10.0f;
	m_cohesionDistance = 10.0f;
}

// Implementation
//===============

void eae6320::cBoidSystem::Update(float i_deltaTime)
{
	Math::sVector goalPosition = m_goal->GetPosition();

	for (auto component = m_componentManager->begin<cBoidComponent>(); component != m_componentManager->end<cBoidComponent>(); ++component)
	{
		cBoidComponent* boidComponent = dynamic_cast<cBoidComponent*>(component->second);

		if (boidComponent && boidComponent->IsActive())
		{
			//Math::sVector desiredVelocity = goalPosition - boidComponent->GetPosition();
			Math::sVector acceleration = CalculateSeparation(boidComponent);
			acceleration += CalculateCohesion(boidComponent);
			boidComponent->SetAcceleration(acceleration);
		}
	}
}

eae6320::Math::sVector eae6320::cBoidSystem::CalculateSeparation(cBoidComponent* i_boidComponent)
{
	Math::sVector seperation = Math::sVector();
	for (auto component = m_componentManager->begin<cBoidComponent>(); component != m_componentManager->end<cBoidComponent>(); ++component)
	{
		cBoidComponent* boidComponent = dynamic_cast<cBoidComponent*>(component->second);

		if (boidComponent && boidComponent->IsActive() && boidComponent != i_boidComponent)
		{
			Math::sVector distance = i_boidComponent->GetPosition() - boidComponent->GetPosition();
			if (distance.GetLength() < m_seperationDistance)
			{
				seperation += distance;
			}
		}
	}
	return seperation;
 }

eae6320::Math::sVector eae6320::cBoidSystem::CalculateCohesion(cBoidComponent* i_boidComponent)
{
	Math::sVector cohesion = Math::sVector();
	float count = 1;
	for (auto component = m_componentManager->begin<cBoidComponent>(); component != m_componentManager->end<cBoidComponent>(); ++component)
	{
		cBoidComponent* boidComponent = dynamic_cast<cBoidComponent*>(component->second);

		if (boidComponent && boidComponent->IsActive() && boidComponent != i_boidComponent)
		{
			Math::sVector distance = i_boidComponent->GetPosition() - boidComponent->GetPosition();
			if (distance.GetLength() < m_cohesionDistance)
			{
				cohesion += boidComponent->GetPosition();
				count++;
			}
		}

		cohesion /= count;
	}
	return cohesion - i_boidComponent->GetPosition();
}

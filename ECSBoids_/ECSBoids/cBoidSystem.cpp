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
	m_goal= *m_componentManager->begin<cGoalComponent>();

	m_neighborDistance = 10.0f;
	m_seperationDistance = 10.0f;
	m_cohesionDistance = 10.0f;
}

// Implementation
//===============

void eae6320::cBoidSystem::Update(float i_deltaTime)
{
	Math::sVector goalPosition = m_goal->GetPosition();

	for (auto boidComponent = m_componentManager->begin<cBoidComponent>(); boidComponent != m_componentManager->end<cBoidComponent>(); ++boidComponent)
	{
		if (*boidComponent && (*boidComponent)->IsActive())
		{
			std::vector<cBoidComponent*> neighbors;
			CalculateNeighbors(neighbors, *boidComponent);

			//Math::sVector desiredVelocity = goalPosition - boidComponent->GetPosition();
			Math::sVector acceleration = CalculateSeparation(neighbors, *boidComponent);
			acceleration += CalculateCohesion(neighbors, *boidComponent);
			(*boidComponent)->SetAcceleration(acceleration);
		}
	}
}

void eae6320::cBoidSystem::CalculateNeighbors(std::vector<cBoidComponent*>& o_neighbors, cBoidComponent* i_boidComponent)
{
	for (auto boidComponent = m_componentManager->begin<cBoidComponent>(); boidComponent != m_componentManager->end<cBoidComponent>(); ++boidComponent)
	{
		if (*boidComponent && (*boidComponent)->IsActive() && (*boidComponent) != i_boidComponent)
		{
			Math::sVector distance = i_boidComponent->GetPosition() - (*boidComponent)->GetPosition();
			if (distance.GetLength() < m_neighborDistance)
			{
				o_neighbors.push_back(*boidComponent);
			}
		}
	}
}


eae6320::Math::sVector eae6320::cBoidSystem::CalculateSeparation(const std::vector<cBoidComponent*>& i_neighbors, cBoidComponent* i_boidComponent)
{
	Math::sVector seperation = Math::sVector();
	for (auto boidComponent = i_neighbors.begin(); boidComponent != i_neighbors.end(); ++boidComponent)
	{
		if (*boidComponent && (*boidComponent)->IsActive() && (*boidComponent) != i_boidComponent)
		{
			Math::sVector distance = i_boidComponent->GetPosition() - (*boidComponent)->GetPosition();
			if (distance.GetLength() < m_seperationDistance)
			{
				seperation += distance;
			}
		}
	}
	return seperation;
 }

eae6320::Math::sVector eae6320::cBoidSystem::CalculateCohesion(const std::vector<cBoidComponent*>& i_neighbors, cBoidComponent* i_boidComponent)
{
	Math::sVector cohesion = Math::sVector();
	float count = 1;
	for (auto boidComponent = i_neighbors.begin(); boidComponent != i_neighbors.end(); ++boidComponent)
	{
		if (*boidComponent && (*boidComponent)->IsActive() && *boidComponent != i_boidComponent)
		{
			Math::sVector distance = i_boidComponent->GetPosition() - (*boidComponent)->GetPosition();
			if (distance.GetLength() < m_cohesionDistance)
			{
				cohesion += (*boidComponent)->GetPosition();
				count++;
			}
		}

		cohesion /= count;
	}
	return cohesion - i_boidComponent->GetPosition();
}

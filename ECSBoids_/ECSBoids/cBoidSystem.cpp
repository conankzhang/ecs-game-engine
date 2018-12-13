// Includes
//=========

#include "cBoidSystem.h"
#include "cBoidComponent.h"
#include "cGoalComponent.h"

#include <Engine/ControllerInput/ControllerInput.h>
#include <Engine/UserInput/UserInput.h>

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

	m_neighborDistance = 5.0f;
	m_seperationDistance = 5.0f;
	m_cohesionDistance = 5.0f;

	m_seperationStrength = 20.0f;
	m_cohesionStrength = 1.0f;
	m_goalStrength = 20.0f;

	m_changeSpeed = 5.0f;
}

void eae6320::cBoidSystem::PreUpdate(float i_deltaTime)
{
	if (UserInput::ControllerInput::IsKeyPressed(UserInput::ControllerInput::ControllerKeyCodes::DPAD_UP) ||
	UserInput::IsKeyPressed(UserInput::KeyCodes::PageUp)
	)
	{
		m_cohesionStrength += m_changeSpeed; 
	}

	if (UserInput::ControllerInput::IsKeyPressed(UserInput::ControllerInput::ControllerKeyCodes::DPAD_DOWN) ||
		UserInput::IsKeyPressed(UserInput::KeyCodes::PageDown)
		)
	{
		m_cohesionStrength -= m_changeSpeed;
	}

	if (UserInput::ControllerInput::IsKeyPressed(UserInput::ControllerInput::ControllerKeyCodes::DPAD_RIGHT) ||
	UserInput::IsKeyPressed(UserInput::KeyCodes::Home)
	)
	{
		m_goalStrength += m_changeSpeed; 
	}

	if (UserInput::ControllerInput::IsKeyPressed(UserInput::ControllerInput::ControllerKeyCodes::DPAD_LEFT) ||
		UserInput::IsKeyPressed(UserInput::KeyCodes::End)
		)
	{
		m_goalStrength -= m_changeSpeed;
	}
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

			Math::sVector goalInfluence = goalPosition - (*boidComponent)->GetPosition();
			goalInfluence.Normalize();

			Math::sVector acceleration = CalculateSeparation(neighbors, *boidComponent);
			acceleration += CalculateCohesion(neighbors, *boidComponent);
			acceleration += goalInfluence * m_goalStrength;
			(*boidComponent)->SetAcceleration(acceleration);
			(*boidComponent)->SetAngularSpeedBasedOnPosition(goalInfluence * m_goalStrength);
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
			Math::sVector distanceVector = i_boidComponent->GetPosition() - (*boidComponent)->GetPosition();
			float distance = distanceVector.GetLength();
			if (distance < m_seperationDistance)
			{
				if (distance > 0)
				{
					distanceVector.Normalize();
					distanceVector /= distance;
				}

				seperation += distanceVector;
			}
		}
	}
	return seperation * m_seperationStrength;
 }

eae6320::Math::sVector eae6320::cBoidSystem::CalculateCohesion(const std::vector<cBoidComponent*>& i_neighbors, cBoidComponent* i_boidComponent)
{
	Math::sVector cohesion = i_boidComponent->GetPosition();
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
	}

	cohesion /= count;
	return (cohesion - i_boidComponent->GetPosition()) * m_cohesionStrength;
}

// Includes
//=========

#include "cBoidSystem.h"
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

}

// Implementation
//===============

void eae6320::cBoidSystem::Update(float i_deltaTime)
{

}

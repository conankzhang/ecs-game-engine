/*
	Physics System implementation.
*/

// Includes
//=========

#include "cPhysicsSystem.h"
#include "cRigidbodyComponent.h"

#include <Engine/Platform/Platform.h>

// Class Implementation 
//==================

eae6320::cPhysicsSystem::cPhysicsSystem()
{

}

eae6320::cPhysicsSystem::~cPhysicsSystem()
{

}

void eae6320::cPhysicsSystem::PreUpdate(float deltaTime)
{
	for (auto component = m_componentManager->begin(); component != m_componentManager->end(); ++component)
	{
		cRigidbodyComponent* rigidbodyComponent = dynamic_cast<cRigidbodyComponent*>(component->second);
		if (rigidbodyComponent && rigidbodyComponent->IsActive())
		{
			rigidbodyComponent->Update(deltaTime);

		}
	}
}

void eae6320::cPhysicsSystem::Update(float deltaTime)
{

}

void eae6320::cPhysicsSystem::PostUpdate(float deltaTime)
{

}

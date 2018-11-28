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
	for (auto rigidBodyComponent = m_componentManager->begin<cRigidbodyComponent>(); rigidBodyComponent != m_componentManager->end<cRigidbodyComponent>(); ++rigidBodyComponent)
	{
		if (*rigidBodyComponent && (*rigidBodyComponent)->IsActive())
		{
			(*rigidBodyComponent)->Update(deltaTime);
		}
	}
}

void eae6320::cPhysicsSystem::Update(float deltaTime)
{

}

void eae6320::cPhysicsSystem::PostUpdate(float deltaTime)
{

}

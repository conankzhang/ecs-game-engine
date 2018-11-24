// Includes
//=========

#include "cInputSystem.h"

#include "cCameraComponent.h"

// Initialization / Clean Up
//--------------------------

eae6320::cInputSystem::cInputSystem()
{

}

eae6320::cInputSystem::~cInputSystem()
{

}

void eae6320::cInputSystem::Initialize()
{
	auto cameraIterator = m_componentManager->begin<cCameraComponent>();
	m_cameraComponent = dynamic_cast<cCameraComponent*>(cameraIterator->second);
}

void eae6320::cInputSystem::UpdateInput()
{
	m_cameraComponent->UpdateInput();
}

// Implementation
//===============

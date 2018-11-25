// Includes
//=========

#include "cInputSystem.h"

#include "cCameraComponent.h"
#include "cGoalComponent.h"

#include <Engine/ControllerInput/ControllerInput.h>

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

	auto goalIterator = m_componentManager->begin<cGoalComponent>();
	m_goalComponent = dynamic_cast<cGoalComponent*>(goalIterator->second);

	m_aIsPressed = false;
	m_cameraFollowSpeed = 5.0f;
}

// Implementation
//===============

void eae6320::cInputSystem::UpdateInput()
{
	m_cameraComponent->UpdateInput();

	if (UserInput::ControllerInput::IsKeyPressed(UserInput::ControllerInput::ControllerKeyCodes::A))
	{
		m_aIsPressed = true;
		m_goalComponent->SetVelocity( (m_cameraComponent->GetPositionInFrontOfCamera(5.0f) - m_goalComponent->GetPosition() ) * m_cameraFollowSpeed);
	}

	if (m_aIsPressed && !UserInput::ControllerInput::IsKeyPressed(UserInput::ControllerInput::ControllerKeyCodes::A))
	{
		m_aIsPressed = false;
		m_goalComponent->SetVelocity(Math::sVector());
	}
}

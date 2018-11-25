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
	m_goalFollowsCamera = false;
	m_cameraFollowSpeed = 5.0f;
}

// Implementation
//===============

void eae6320::cInputSystem::UpdateInput()
{
	m_cameraComponent->UpdateInput();
	HandleFollowToggle();
}

void eae6320::cInputSystem::HandleFollowToggle()
{
	if (!m_aIsPressed && UserInput::ControllerInput::IsKeyPressed(UserInput::ControllerInput::ControllerKeyCodes::A))
	{
		m_aIsPressed = true;
		m_goalFollowsCamera = !m_goalFollowsCamera;

		if (!m_goalFollowsCamera)
		{
			m_goalComponent->SetVelocity(Math::sVector());
		}
	}

	if (m_aIsPressed && !UserInput::ControllerInput::IsKeyPressed(UserInput::ControllerInput::ControllerKeyCodes::A))
	{
		m_aIsPressed = false;
	}

	if (m_goalFollowsCamera)
	{
		m_goalComponent->SetVelocity( (m_cameraComponent->GetPositionInFrontOfCamera(5.0f) - m_goalComponent->GetPosition() ) * m_cameraFollowSpeed);
	}
}

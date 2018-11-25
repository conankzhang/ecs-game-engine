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

	m_goalFollowDistance = 5.0f;
	m_minGoalFollowDistance = m_goalFollowDistance;
	m_maxGoalFollowDistance = 50.0f;
	m_changeDistanceSpeed = 0.1f;
}

// Implementation
//===============

void eae6320::cInputSystem::UpdateInput()
{
	m_cameraComponent->UpdateInput();
	HandleFollowDistance();
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
		m_goalComponent->SetVelocity( (m_cameraComponent->GetPositionInFrontOfCamera(m_goalFollowDistance) - m_goalComponent->GetPosition() ) * m_cameraFollowSpeed);
	}
}

void eae6320::cInputSystem::HandleFollowDistance()
{
	if (!m_goalFollowsCamera)
	{
		return;
	}

	if (UserInput::ControllerInput::IsKeyPressed(UserInput::ControllerInput::ControllerKeyCodes::RIGHT_SHOULDER))
	{
		if (m_goalFollowDistance < m_maxGoalFollowDistance)
		{
			m_goalFollowDistance += m_changeDistanceSpeed;
		}
		else
		{
			m_goalFollowDistance = m_maxGoalFollowDistance;
		}
	}

	if (UserInput::ControllerInput::IsKeyPressed(UserInput::ControllerInput::ControllerKeyCodes::LEFT_SHOULDER))
	{
		if (m_goalFollowDistance > m_minGoalFollowDistance)
		{
			m_goalFollowDistance -= m_changeDistanceSpeed;
		}
		else
		{
			m_goalFollowDistance = m_minGoalFollowDistance;
		}
	}
}

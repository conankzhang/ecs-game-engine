// Includes
//=========

#include "cInputSystem.h"

#include "cCameraComponent.h"
#include "cGoalComponent.h"

#include <Engine/ControllerInput/ControllerInput.h>
#include <Engine/UserInput/UserInput.h>

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
	m_cameraComponent = *m_componentManager->begin<cCameraComponent>();

	m_goalComponent = *m_componentManager->begin<cGoalComponent>();

	m_aIsPressed = false;
	m_spaceIsPressed = false;
	m_goalFollowsCamera = false;
	m_cameraFollowSpeed = 10.0f;

	m_goalFollowDistance = 5.0f;
	m_minGoalFollowDistance = m_goalFollowDistance;
	m_maxGoalFollowDistance = 50.0f;
	m_changeDistanceSpeed = 0.01f;
}

// Implementation
//===============

void eae6320::cInputSystem::UpdateInput()
{
	m_cameraComponent->UpdateInput();
	HandleFollowDistance();
	HandleFollowToggle();

	if (m_goalFollowsCamera)
	{
		m_goalComponent->SetVelocity( (m_cameraComponent->GetPositionInFrontOfCamera(m_goalFollowDistance) - m_goalComponent->GetPosition() ) * m_cameraFollowSpeed);
	}
}

void eae6320::cInputSystem::HandleFollowToggle()
{
	if (!m_aIsPressed && UserInput::ControllerInput::IsKeyPressed(UserInput::ControllerInput::ControllerKeyCodes::A))
	{
		m_aIsPressed = true;
		OnFollowToggle();
	}

	if (m_aIsPressed && !UserInput::ControllerInput::IsKeyPressed(UserInput::ControllerInput::ControllerKeyCodes::A))
	{
		m_aIsPressed = false;
	}

	if (!m_spaceIsPressed && UserInput::IsKeyPressed(UserInput::KeyCodes::Space))
	{
		m_spaceIsPressed = true;
		OnFollowToggle();
	}

	if (m_spaceIsPressed && !UserInput::IsKeyPressed(UserInput::KeyCodes::Space))
	{
		m_spaceIsPressed = false;
	}
}

void eae6320::cInputSystem::OnFollowToggle()
{
	m_goalFollowsCamera = !m_goalFollowsCamera;

	if (!m_goalFollowsCamera)
	{
		m_goalComponent->SetVelocity(Math::sVector());
	}
}

void eae6320::cInputSystem::HandleFollowDistance()
{
	if (!m_goalFollowsCamera)
	{
		return;
	}

	if (UserInput::ControllerInput::IsKeyPressed(UserInput::ControllerInput::ControllerKeyCodes::RIGHT_SHOULDER) ||
		UserInput::IsKeyPressed(UserInput::KeyCodes::Up)
		)
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

	if (UserInput::ControllerInput::IsKeyPressed(UserInput::ControllerInput::ControllerKeyCodes::LEFT_SHOULDER) ||
		UserInput::IsKeyPressed(UserInput::KeyCodes::Down)
		)
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

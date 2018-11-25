// Includes
//=========

#include "cECSBoids.h"

#include "cBoid.h"
#include "cGoal.h"
#include "cCamera.h"

#include "cRenderComponent.h"
#include "cCameraComponent.h"

#include "cRenderSystem.h"
#include "cInputSystem.h"
#include "cBoidSystem.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include <Engine/ControllerInput/ControllerInput.h>

// Inherited Implementation
//=========================

void eae6320::cECSBoids::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	ECS->GetSystemManager()->GetSystem<cRenderSystem>()->SubmitDataToBeRendered(i_elapsedSecondCount_systemTime, i_elapsedSecondCount_sinceLastSimulationUpdate);
}

// Run
//----

void eae6320::cECSBoids::UpdateBasedOnInput()
{
	// Is the user pressing the ESC key?
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) || UserInput::ControllerInput::IsKeyPressed(UserInput::ControllerInput::ControllerKeyCodes::START))
	{
		// Exit the application
		const auto result = Exit( EXIT_SUCCESS );
		EAE6320_ASSERT( result );
	}

	if (!m_bIsPressed && UserInput::ControllerInput::IsKeyPressed(UserInput::ControllerInput::ControllerKeyCodes::B))
	{
		m_bIsPressed = true;
		ECS->GetEntityManager()->CreateEntity<cBoid>();
	}

	if (m_bIsPressed && !UserInput::ControllerInput::IsKeyPressed(UserInput::ControllerInput::ControllerKeyCodes::B))
	{
		m_bIsPressed = false;
	}

	ECS->GetSystemManager()->GetSystem<cInputSystem>()->UpdateInput();
}

void eae6320::cECSBoids::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	ECS->Update(i_elapsedSecondCount_sinceLastUpdate);
}

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::cECSBoids::Initialize()
{
	cResult result = Results::Success;

	if (!(result = UserInput::ControllerInput::Initialize()))
	{
		EAE6320_ASSERT(false);
		goto OnExit;
	}

	
	ECS = new ECS::ECSEngine();

	size_t cameraId = ECS->GetEntityManager()->CreateEntity<cCamera>();
	ECS->GetEntityManager()->CreateEntity<cGoal>(ECS->GetEntityManager()->GetEntity(cameraId)->GetComponent<cCameraComponent>());

	cRenderSystem* renderSystem = ECS->GetSystemManager()->AddSystem<cRenderSystem>();
	cInputSystem* inputSystem = ECS->GetSystemManager()->AddSystem<cInputSystem>();
	cBoidSystem* boidSystem = ECS->GetSystemManager()->AddSystem<cBoidSystem>();

	ECS->GetSystemManager()->AddSystemDependency<cBoidSystem*, cInputSystem*>(boidSystem, inputSystem);
	ECS->GetSystemManager()->AddSystemDependencies<cRenderSystem*, cInputSystem*, cBoidSystem*>(renderSystem, inputSystem, boidSystem);

OnExit:
	return result;
}

eae6320::cResult eae6320::cECSBoids::CleanUp()
{
	delete ECS;

	UserInput::ControllerInput::CleanUp();

	return Results::Success;
}

// Includes
//=========

#include "cECSBoids.h"

#include "cBoid.h"
#include "cCamera.h"
#include "cRenderSystem.h"
#include "cRenderComponent.h"

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
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) )
	{
		// Exit the application
		const auto result = Exit( EXIT_SUCCESS );
		EAE6320_ASSERT( result );
	}
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

	ECS->GetEntityManager()->CreateEntity<cBoid>();
	ECS->GetEntityManager()->CreateEntity<cCamera>();

	ECS->GetSystemManager()->AddSystem<cRenderSystem>();

OnExit:
	return result;
}

eae6320::cResult eae6320::cECSBoids::CleanUp()
{
	delete ECS;

	UserInput::ControllerInput::CleanUp();

	return Results::Success;
}

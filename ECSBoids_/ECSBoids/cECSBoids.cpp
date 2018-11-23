// Includes
//=========

#include "cECSBoids.h"

#include "cBoid.h"
#include "cRenderSystem.h"
#include "cRenderComponent.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Graphics/Graphics.h>
#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/UserInput/UserInput.h>

// Inherited Implementation
//=========================

void eae6320::cECSBoids::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	Graphics::SubmitBackgroundColor(0.13f, 0.24f, 0.33f, 1.0f);

	for (auto component = ECS->GetComponentManager()->begin<cRenderComponent>(); component != ECS->GetComponentManager()->end<cRenderComponent>(); ++component)
	{
		cRenderComponent* renderComponent = dynamic_cast<cRenderComponent*>(component->second);

		if (renderComponent && renderComponent->IsActive())
		{
			Graphics::SubmitGameObject(renderComponent->GetMesh(), renderComponent->GetEffect() , renderComponent->GetTransform(i_elapsedSecondCount_sinceLastSimulationUpdate));
		}
	}
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
	ECS = new ECS::ECSEngine();

	ECS->GetEntityManager()->CreateEntity<cBoid>();

	ECS->GetSystemManager()->AddSystem<cRenderSystem>();
	return Results::Success;
}

eae6320::cResult eae6320::cECSBoids::CleanUp()
{
	delete ECS;
	return Results::Success;
}

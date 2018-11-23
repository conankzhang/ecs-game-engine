// Includes
//=========

#include "cECSBoids.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>

// Inherited Implementation
//=========================

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

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::cECSBoids::Initialize()
{
	return Results::Success;
}

eae6320::cResult eae6320::cECSBoids::CleanUp()
{
	return Results::Success;
}

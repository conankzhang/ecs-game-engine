// Includes
//=========

#include "cEvent.h"

#include <Engine/Asserts/Asserts.h>

// Interface
//==========

// Initialization / Clean Up
//--------------------------

eae6320::Concurrency::cEvent::~cEvent()
{
	const auto result = CleanUp();
	EAE6320_ASSERT( result );
}

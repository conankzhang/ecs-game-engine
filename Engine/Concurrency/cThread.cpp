// Includes
//=========

#include "cThread.h"

#include <Engine/Asserts/Asserts.h>

// Interface
//==========

// Initialization / Clean Up
//--------------------------

eae6320::Concurrency::cThread::~cThread()
{
	const auto result = CleanUp();
	EAE6320_ASSERT( result );
}

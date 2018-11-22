/*
	ISystem implementation.
*/

// Includes
//=========

#include "ISystem.h"

// Forward Declarations
//=====================

// Class Declaration
//==================

namespace eae6320
{
	namespace ECS 
	{
		ISystem::ISystem(size_t i_priority, float i_updateInterval) :
			m_systemPriority(i_priority),
			m_updateInterval(i_updateInterval),
			m_isActive(true)
		{

		}

		ISystem::~ISystem()
		{

		}
	}
}

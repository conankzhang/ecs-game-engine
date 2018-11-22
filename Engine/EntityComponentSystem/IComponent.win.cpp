/*
	IComponent implementation.
*/

// Includes
//=========

#include "IComponent.h"

// Forward Declarations
//=====================

// Class Declaration
//==================

namespace eae6320
{
	namespace ECS 
	{
		IComponent::IComponent() :	
			m_ownerId(0),
			m_isActive(true)
		{

		}

		IComponent::~IComponent()
		{

		}
	}
}

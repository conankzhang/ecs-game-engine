/*
	IEntity implementation.
*/

// Includes
//=========

#include "IEntity.h"

// Forward Declarations
//=====================

// Class Declaration
//==================

namespace eae6320
{
	namespace ECS 
	{
		IEntity::IEntity() : 
			m_isActive(true)
		{

		}

		IEntity::~IEntity()
		{

		}

		void IEntity::SetIsActive(bool i_isActive)
		{
			if (this->m_isActive == i_isActive)
			{
				return;
			}

			if (i_isActive == false)
			{
				OnDisable();
			}
			else
			{
				OnEnable();
			}

			m_isActive = i_isActive;
		}
	}
}

/*
	System manager implementation.
*/

// Includes
//=========

#include "SystemManager.h"

// Forward Declarations
//=====================

// Class Declaration
//==================

namespace eae6320
{
	namespace ECS 
	{
		SystemManager::SystemManager(ComponentManager* i_componentManager) :
			m_componentManager(i_componentManager)
		{
		}
		
		SystemManager::~SystemManager()
		{		
			for (std::vector<ISystem*>::reverse_iterator it = m_systemOrder.rbegin(); it != m_systemOrder.rend(); ++it)
			{
				(*it)->~ISystem();
				*it = nullptr;
			}

			m_systemOrder.clear();
			m_systems.clear();
		}

		void SystemManager::Update(float deltaTime)
		{
			for (ISystem* system : m_systemOrder)
			{
				system->m_deltaTime += deltaTime;

				if (system->m_isActive == true)
				{			
					system->PreUpdate(deltaTime);	
				}
			}

			for (ISystem* system : m_systemOrder)
			{
				if (system->m_isActive == true)
				{
					system->Update(deltaTime);
					system->m_deltaTime = 0.0f;
				}
			}

			for (ISystem* system : m_systemOrder)
			{
				if (system->m_isActive == true)
				{
					system->PostUpdate(deltaTime);
				}
			}
		}
	}
}

/*
	Entity manager implementation.
*/

// Includes
//=========

#include "EntityManager.h"

// Forward Declarations
//=====================

// Class Declaration
//==================

namespace eae6320
{
	namespace ECS
	{
		EntityManager::EntityManager(ComponentManager* i_componentManager) :
			m_entitiesToRemove(1024),
			m_numEntitiesToRemove(0),
			m_componentManager(i_componentManager)
		{

		}

		EntityManager::~EntityManager()
		{
			for (auto entity : m_entityMap)
			{
				delete entity.second;
				entity.second = nullptr;
			}
		}

		void EntityManager::RemoveEntities()
		{
			for (auto entity : m_entitiesToRemove)
			{
				m_entityMap.erase(entity);
			}

			m_numEntitiesToRemove = 0;
		}
	}
}

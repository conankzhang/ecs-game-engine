/*
	Entity manager.
*/

#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H 

// Includes
//=========

#include "IEntity.h"
#include "ComponentManager.h"

#include <unordered_map>

// Forward Declarations
//=====================

// Class Declaration
//==================

namespace eae6320
{
	namespace ECS 
	{
		class EntityManager 
		{
		public:
			EntityManager(ComponentManager* i_componentManager);
			~EntityManager();

			// Create an Entity of type T with optional arguments
			template<class T, class... ARGS>
			size_t CreateEntity(ARGS&&... args)
			{		
				void* entity = new T(std::forward<ARGS>(args)...);

				size_t entityId = ++m_entityCount;

				((T*)entity)->m_entityId = entityId;
				((T*)entity)->m_componentManager = m_componentManager;

				m_entityMap[entityId] = static_cast<IEntity*>(entity);

				((T*)entity)->Initialize();

				return entityId;
			}

			// Destroy the Entity with the specified EntityID
			void DestroyEntity(size_t i_entityId)
			{
				if (m_numEntitiesToRemove < m_entitiesToRemove.size())
				{
					m_entitiesToRemove[m_numEntitiesToRemove++] = i_entityId;
				}
				else
				{
					m_entitiesToRemove.push_back(i_entityId);
					m_numEntitiesToRemove++;
				}
			}

			// Get the Entity with the specified EntityID
			inline IEntity* GetEntity(size_t i_entityId)
			{
				return m_entityMap[i_entityId];
			}

			// Remove Entities that were marked to be destroyed 
			void RemoveEntities();

		protected:

		private:
			ComponentManager* m_componentManager;

			std::unordered_map<size_t, IEntity*> m_entityMap;
			size_t m_entityCount;

			std::vector<size_t> m_entitiesToRemove;
			size_t m_numEntitiesToRemove;
			
		};
	}
}

#endif	// ENTITYMANAGER_H

/*
	Component manager.
*/

#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H 

// Includes
//=========

#include "IComponent.h"

#include <unordered_map>

// Forward Declarations
//=====================

// Class Declaration
//==================

namespace eae6320
{
	namespace ECS 
	{
		class ComponentManager 
		{
			friend class IComponent;
		public:
			ComponentManager();
			~ComponentManager();

			ComponentManager(const ComponentManager&) = delete;
			ComponentManager& operator=(ComponentManager&) = delete;

			// Add a Component of type T with optional arguments to the Entity with EntityID 
			template<class T, class ...ARGS>
			T* AddComponent(const size_t i_entityId, ARGS&&... args)
			{
				void* component = new T(std::forward<ARGS>(args)...);

				const size_t componentTypeId = T::s_componentTypeId;

				size_t componentId = ++m_componentCount;

				((T*)component)->m_ownerId = i_entityId;
				((T*)component)->m_componentId = componentId;

				m_entityComponentMap[i_entityId][componentTypeId] = componentId;
				m_componentMap[componentTypeId][componentId] = static_cast<T*>(component);
				return static_cast<T*>(component);
			}

			// Remove the Component of type T frome the Entity with EntityID 
			template<class T>
			void RemoveComponent(const size_t i_entityId)
			{
				const size_t componentTypeId = T::s_componentTypeId;

				const size_t componentId = m_entityComponentMap[i_entityId][componentTypeId];

				m_componentMap[componentTypeId].erase(componentId);

				m_entityComponentMap[i_entityId][componentTypeId] = 0;

			}

			// Get the Component of type T frome the Entity with EntityID 
			template<class T>
			T* GetComponent(const size_t i_entityId)
			{
				const size_t componentTypeId = T::s_componentTypeId;

				const size_t componentId = m_entityComponentMap[i_entityId][componentTypeId];

				return static_cast<T*>(this->m_componentMap[componentTypeId][componentId]);
			}

			// Get the begin iterator for Components of type T
			template<class T>
			inline std::unordered_map<size_t, IComponent*>::iterator begin() 
			{
				const size_t componentTypeId = T::s_componentTypeId;
				return m_componentMap[componentTypeId].begin();
			}

			// Get the end iterator for Components of type T
			template<class T>
			inline std::unordered_map<size_t, IComponent*>::iterator end() 
			{
				const size_t componentTypeId = T::s_componentTypeId;
				return m_componentMap[componentTypeId].end();
			}
		private:
			std::vector<std::vector<size_t>> m_entityComponentMap;
			std::unordered_map<size_t, std::unordered_map<size_t, IComponent*>> m_componentMap;
			size_t m_componentCount;
		};
	}
}

#endif	// COMPONENTMANAGER_H

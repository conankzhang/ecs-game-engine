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

			template<class T, class ...ARGS>
			T* AddComponent(const size_t i_entityId, ARGS&&... args)
			{
				void* component = new T(std::forward<ARGS>(args)...);

				const size_t componentTypeId = T::s_componentTypeId;

				size_t componentId = ++m_componentCount;

				((T*)component)->m_ownerId = i_entityId;
				((T*)component)->m_componentId = componentId;

				m_entityComponentMap[i_entityId][componentTypeId] = componentId;

				m_componentMap[componentId] = static_cast<IComponent*>(component);
				return static_cast<T*>(component);
			}

			template<class T>
			void RemoveComponent(const size_t i_entityId)
			{
				const size_t componentTypeId = T::s_componentTypeId;

				const size_t componentId = m_entityComponentMap[i_entityId][componentTypeId];

				IComponent* component = m_componentMap[componentId];

				delete component;

				m_entityComponentMap[i_entityId][componentTypeId] = 0;

			}

			template<class T>
			T* GetComponent(const size_t i_entityId)
			{
				const size_t componentTypeId = T::s_componentTypeId;

				const size_t componentId = m_entityComponentMap[i_entityId][componentTypeId];

				return static_cast<T*>(this->m_componentMap[componentId]);
			}

			inline std::unordered_map<size_t, IComponent*>::iterator begin() 
			{
				return m_componentMap.begin();
			}

			inline std::unordered_map<size_t, IComponent*>::iterator end() 
			{
				return m_componentMap.end();
			}
		private:
			std::vector<std::vector<size_t>> m_entityComponentMap;
			std::unordered_map<size_t, IComponent*> m_componentMap;
			size_t m_componentCount;
			
		};
	}
}

#endif	// COMPONENTMANAGER_H

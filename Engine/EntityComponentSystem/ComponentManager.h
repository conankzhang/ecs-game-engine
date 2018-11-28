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
			class IComponentContainer
			{
			public:
			
				virtual ~IComponentContainer()
				{}
			};
	
			template<class T>
			class ComponentContainer: public IComponentContainer
			{
				ComponentContainer(const ComponentContainer&) = delete;
				ComponentContainer& operator=(ComponentContainer&) = delete;

			public:
			
				ComponentContainer()
				{}

				virtual ~ComponentContainer()
				{}

				template<class... ARGS>
				T* CreateComponent(ARGS&&... args)
				{
					void* component = new T(std::forward<ARGS>(args)...);
					m_components.push_back((T*)component);
					return (T*)component;
				}

				void DestroyComponent(T* component)
				{		
					m_components.erase(component);
				}

				inline typename std::vector<T*>::iterator begin()
				{
					return m_components.begin();
				}

				inline typename std::vector<T*>::iterator end()
				{
					return m_components.end();
				}

				std::vector<T*> m_components;
			};

			template<class T>
			inline ComponentContainer<T>* GetComponentContainer()
			{
				size_t componentTypeId = T::s_componentTypeId;;

				auto iterator = m_componentContainerMap.find(componentTypeId);
				ComponentContainer<T>* componentContainer = nullptr;

				if (iterator == m_componentContainerMap.end())
				{
					componentContainer = new ComponentContainer<T>();
					m_componentContainerMap[componentTypeId] = componentContainer;
				}
				else
				{
					componentContainer = static_cast<ComponentContainer<T>*>(iterator->second);
				}

				return componentContainer;
			}
		public:
			ComponentManager();
			~ComponentManager();

			ComponentManager(const ComponentManager&) = delete;
			ComponentManager& operator=(ComponentManager&) = delete;

			// Add a Component of type T with optional arguments to the Entity with EntityID 
			template<class T, class ...ARGS>
			T* AddComponent(const size_t i_entityId, ARGS&&... args)
			{
				T* component = GetComponentContainer<T>()->CreateComponent(std::forward<ARGS>(args)...);

				const size_t componentTypeId = T::s_componentTypeId;

				size_t componentId = ++m_componentCount;

				component->m_ownerId = i_entityId;
				component->m_componentId = componentId;

				m_entityComponentMap[i_entityId][componentTypeId] = componentId;
				m_components[componentId] = component;
				return component;
			}

			// Remove the Component of type T from the Entity with EntityID 
			template<class T>
			void RemoveComponent(const size_t i_entityId)
			{
				const size_t componentTypeId = T::s_componentTypeId;

				const size_t componentId = m_entityComponentMap[i_entityId][componentTypeId];

				m_entityComponentMap[i_entityId][componentTypeId] = 0;
				GetComponentContainer<T>()->DestroyComponent(m_components[componentId]);
			}

			// Get the Component of type T frome the Entity with EntityID 
			template<class T>
			T* GetComponent(const size_t i_entityId)
			{
				const size_t componentTypeId = T::s_componentTypeId;

				const size_t componentId = m_entityComponentMap[i_entityId][componentTypeId];

				return static_cast<T*>(this->m_components[componentId]);
			}

			// Get the begin iterator for Components of type T
			template<typename T>
			inline typename std::vector<T*>::iterator begin()
			{
				return GetComponentContainer<T>()->begin();
			}

			// Get the end iterator for Components of type T
			template<typename T>
			inline typename std::vector<T*>::iterator end()
			{
				return GetComponentContainer<T>()->end();
			}

		private:
			std::vector<std::vector<size_t>> m_entityComponentMap;
			std::unordered_map<size_t, IComponentContainer*> m_componentContainerMap;
			std::vector<IComponent*> m_components;
			size_t m_componentCount;
		};
	}
}

#endif	// COMPONENTMANAGER_H

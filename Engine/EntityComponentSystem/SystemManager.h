/*
	System Manager.
*/

#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H 

// Includes
//=========

#include "ISystem.h"
#include "Engine.h"
#include "ComponentManager.h"

#include <vector>
#include <unordered_map>

// Forward Declarations
//=====================

// Class Declaration
//==================

namespace eae6320
{
	namespace ECS 
	{
		class SystemManager
		{
			friend ECSEngine;

		public:
			SystemManager(ComponentManager* i_componentManager);
			~SystemManager();

			// Add a System of type T with optional arguments
			template<class T, class... ARGS>
			T* AddSystem(ARGS&&... args)
			{
				const size_t systemTypeId = T::s_systemTypeId;

				auto it = m_systems.find(systemTypeId);
				if ((m_systems.find(systemTypeId) != m_systems.end()) && (it->second != nullptr))
				{
					return (T*)it->second;
				}

				void* system = new T(std::forward<ARGS>(args)...);

				((T*)system)->m_systemManager = this;
				((T*)system)->m_componentManager = m_componentManager;
				m_systems[systemTypeId] = (T*)system;

				if (systemTypeId + 1 > m_systemDependencies.size())
				{
					m_systemDependencies.resize(systemTypeId + 1);
					for (size_t i = 0; i < m_systemDependencies.size(); ++i)
					{
						m_systemDependencies[i].resize(systemTypeId + 1);
					}
				}

				// add to work list
				this->m_systemOrder.push_back((T*)system);

				((T*)system)->Initialize();

				return (T*)system;
			}

			// Get the System of type T
			template<class T>
			inline T* GetSystem() const
			{
				auto it = this->m_systems.find(T::s_systemTypeId);

				return it != this->m_systems.end() ? (T*)it->second : nullptr;
			}

			// Add a Dependency to System to indicate the dependency should be updated before the system 
			template<class System, class Dependency>
			void AddSystemDependency(System system, Dependency dependency)
			{
				const size_t systemId= system->GetSystemTypeId();
				const size_t dependencyId = dependency->GetSystemTypeId();

				if (this->m_systemDependencies[systemId][dependencyId] != true)
				{
					this->m_systemDependencies[systemId][dependencyId] = true;
				}
			}

			// Add a Dependencies to System to indicate the dependencies should be updated before the system 
			template<class System, class Dependency, class... Dependencies>
			void AddSystemDependencies(System system, Dependency dependency, Dependencies&... dependencies)
			{
				const size_t systemId= system->GetSystemTypeId();
				const size_t dependencyId = dependency->GetSystemTypeId();

				if (this->m_systemDependencies[systemId][dependencyId] != true)
				{
					this->m_systemDependencies[systemId][dependencyId] = true;
				}

				this->AddSystemDependency(system, std::forward<Dependencies>(dependencies)...);
			}

		private:
			void Update(float deltaTime);

		private:
			ComponentManager* m_componentManager;
			std::vector<std::vector<bool>> m_systemDependencies;
			std::unordered_map<size_t, ISystem*> m_systems;
			std::vector<ISystem*> m_systemOrder;

		};
	}
}

#endif	// SYSTEMMANAGER_H

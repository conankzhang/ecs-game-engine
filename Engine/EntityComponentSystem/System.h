/*
	System.
*/

#ifndef SYSTEM_H
#define SYSTEM_H 

// Includes
//=========

#include "ISystem.h"
#include "ComponentManager.h"
#include "TypeManager.h"

// Forward Declarations
//=====================

// Class Declaration
//==================

namespace eae6320
{
	namespace ECS 
	{
		template<class T>
		class System : public ISystem
		{
			friend class SystemManager;

		public:
			virtual ~System() {}

			virtual inline const size_t GetSystemTypeId() const
			{
				return s_systemTypeId;
			}

			template<class... Dependencies>
			void AddDependencies(Dependencies&&... dependencies)
			{
				m_systemManager->AddSystemDependencies(this, std::forward<Dependencies>(dependencies)...);
			}

			virtual void PreUpdate(float deltaTime) override {};
			virtual void Update(float deltaTime) override {};
			virtual void PostUpdate(float deltaTime) override {};

		private:
			SystemManager* m_systemManager;
			static const size_t s_systemTypeId;
		};

		template<class T>
		const size_t System<T>::s_systemTypeId = TypeManager<ISystem>::GetTypeId<T>();
	}
}

#endif	// SYSTEM_H

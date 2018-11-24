/*
	Engine interface.
*/

#ifndef ENGINE_H
#define ENGINE_H 

// Includes
//=========

// Forward Declarations
//=====================
namespace eae6320
{
	namespace ECS
	{
		class EntityManager;
		class SystemManager;
		class ComponentManager;
	}
}

// Class Declaration
//==================

namespace eae6320
{
	namespace ECS 
	{
		class ECSEngine 
		{
			friend class IEntity;
			friend class IComponent;
			friend class ISystem;

			friend class EntityManager;

		public:
			ECSEngine();
			~ECSEngine();

			// Getters for ECS managers
			inline EntityManager* GetEntityManager() { return m_entityManager; }
			inline ComponentManager* GetComponentManager() { return m_componentManager; }
			inline SystemManager* GetSystemManager() { return m_systemManager; }

			void Update(float deltaTime);

		private:
			EntityManager* m_entityManager;
			ComponentManager* m_componentManager;
			SystemManager* m_systemManager;
		};
	}
}

#endif	// ENGINE_H

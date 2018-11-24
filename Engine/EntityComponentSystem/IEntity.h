/*
	Entity interface.
*/

#ifndef IENTITY_H
#define IENTITY_H 

// Includes
//=========

// Forward Declarations
//=====================
namespace eae6320
{
	namespace ECS
	{
		class ComponentManager;
	}
}
// Class Declaration
//==================

namespace eae6320
{
	namespace ECS 
	{
		class IEntity 
		{
			friend class EntityManager;

		public:
			IEntity();
			virtual ~IEntity();

			// Add a Component of type T to this Entity with optional arguments
			template<class T, class ...ARGS>
			T* AddComponent(ARGS&&... args) const 
			{
				return this->m_componentManager->AddComponent<T>(this->GetId(), std::forward<ARGS>(args)...);
			}

			// Remove the Component of type T from this Entity
			template<class T>
			void RemoveComponent()
			{
				return this->m_componentManager->RemoveComponent<T>(this->GetId());
			}

			// Get the Component of type T attached to this Entity
			template<class T>
			T* GetComponent() const 
			{
				return this->m_componentManager->GetComponent<T>(this->GetId());
			}

			// Similar to Start in Unity and BeginPlay in Unreal
			virtual void Initialize() {};

			// Getters and Setters for the active status of the Entity
			inline bool IsActive() const { return this->m_isActive; }
			void SetIsActive(bool i_isActive);

			// Handlers for when SetIsActive is called
			virtual void OnEnable() {}
			virtual void OnDisable() {}

			// Get the unique identifier for this entity
			inline const size_t GetId() const { return this->m_entityId; }

			// Entity equality checks 
			inline bool operator==(const IEntity& rhs) const { return this->GetId() == rhs.GetId(); }
			inline bool operator!=(const IEntity& rhs) const { return this->GetId() != rhs.GetId(); }
			inline bool operator==(const IEntity* rhs) const { return this->GetId() == rhs->GetId(); }
			inline bool operator!=(const IEntity* rhs) const { return this->GetId() != rhs->GetId(); }

		protected:
			size_t m_entityId;
			bool m_isActive;

		private:
			ComponentManager* m_componentManager;
		};
	}
}

#endif	// IENTITY_H

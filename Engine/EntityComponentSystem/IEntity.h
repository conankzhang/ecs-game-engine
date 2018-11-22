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

			template<class T>
			T* GetComponent() const 
			{
				return this->m_componentManager->GetComponent<T>(this->GetId());
			}

			template<class T, class ...ARGS>
			T* AddComponent(ARGS&&... args) const 
			{
				return this->m_componentManager->AddComponent<T>(this->GetId(), std::forward<ARGS>(args)...);
			}

			template<class T>
			void RemoveComponent()
			{
				return this->m_componentManager->RemoveComponent<T>(this->GetId());
			}

			inline bool operator==(const IEntity& rhs) const { return this->GetId() == rhs.GetId(); }
			inline bool operator!=(const IEntity& rhs) const { return this->GetId() != rhs.GetId(); }
			inline bool operator==(const IEntity* rhs) const { return this->GetId() == rhs->GetId(); }
			inline bool operator!=(const IEntity* rhs) const { return this->GetId() != rhs->GetId(); }

			inline const size_t GetId() const { return this->m_entityId; }

			void SetActive(bool i_isActive);
			inline bool IsActive() const { return this->m_isActive; }

			virtual void OnEnable() {}
			virtual void OnDisable() {}

			virtual void Initialize() {};

		protected:
			size_t m_entityId;
			bool m_isActive;

		private:
			ComponentManager* m_componentManager;
		};
	}
}

#endif	// IENTITY_H

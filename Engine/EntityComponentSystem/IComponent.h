/*
	Component interface.
*/

#ifndef ICOMPONENT_H
#define ICOMPONENT_H 

// Includes
//=========

#include "IEntity.h"

// Forward Declarations
//=====================

// Class Declaration
//==================

namespace eae6320
{
	namespace ECS 
	{
		class IComponent
		{
			friend class ComponentManager;

		public:
			IComponent();
			virtual ~IComponent();

			// Get the unique identifier for the Entity that owns this Component
			inline const size_t GetOwner() const { return this->m_ownerId; }

			// Get the unique identifier for this Component
			inline const size_t GetId() const { return this->m_componentId; }

			// Getters and Setters for the active status of the Component 
			inline void SetActive(bool isActive) { this->m_isActive = isActive; }
			inline bool IsActive() const { return this->m_isActive; }

			// Component equality checks
			inline bool operator==(const IComponent & rhs) const { return this->GetId() == rhs.GetId(); }
			inline bool operator!=(const IComponent& rhs) const { return this->GetId() != rhs.GetId(); }

		protected:
			size_t m_componentId;
			size_t m_ownerId;

			bool m_isActive;
		};
	}
}

#endif	// ICOMPONENT_H

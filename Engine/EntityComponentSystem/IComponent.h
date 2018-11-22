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

			inline bool operator==(const IComponent & rhs) const { return this->GetId() == rhs.GetId(); }
			inline bool operator!=(const IComponent& rhs) const { return this->GetId() != rhs.GetId(); }

			inline const size_t GetId() const { return this->m_componentId; }
			inline const size_t GetOwner() const { return this->m_ownerId; }

			inline void SetActive(bool isActive) { this->m_isActive = isActive; }
			inline bool IsActive() const { return this->m_isActive; }

		protected:
			size_t m_componentId;
			size_t m_ownerId;

			bool m_isActive;
		};
	}
}

#endif	// ICOMPONENT_H

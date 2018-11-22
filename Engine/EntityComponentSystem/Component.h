/*
	Component.
*/

#ifndef COMPONENT_H
#define COMPONENT_H 

// Includes
//=========

#include "IComponent.h"

// Forward Declarations
//=====================

// Class Declaration
//==================

namespace eae6320
{
	namespace ECS 
	{
		template<class T>
		class Component : public IComponent
		{
		public:
			Component() {}

			static const size_t s_componentTypeId;
		};

		static size_t s_typeIdCount = 0;

		template<class T>
		const size_t Component<T>::s_componentTypeId = s_typeIdCount++;

	}
}

#endif	// COMPONENT_H

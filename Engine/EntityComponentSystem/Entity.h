/*
	Entity
*/

#ifndef ENTITY_H
#define ENTITY_H 

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
		template<class E>
		class Entity : public IEntity 
		{
		public:
			Entity() {}
			virtual ~Entity() {}
		};
	}
}

#endif	// ENTITY_H

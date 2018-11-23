/*
	Type manager.
*/

// Includes
//=========

#include "TypeManager.h"

// Forward Declarations
//=====================

namespace eae6320
{
	namespace ECS
	{
		class IEntity;
		class IComponent;
		class ISystem;
			
		template class TypeManager<IEntity>;
		template class TypeManager<IComponent>;
		template class TypeManager<ISystem>;		
	}
}

// Class Declaration
//==================

namespace eae6320
{
	namespace ECS 
	{
		size_t TypeManager<IEntity>::s_typeCount = 0;
		size_t TypeManager<IComponent>::s_typeCount = 0;
		size_t TypeManager<ISystem>::s_typeCount = 0;
	}
}

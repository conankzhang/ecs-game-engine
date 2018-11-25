/*
	Floor interface.
*/

#ifndef CFLOOR_H
#define CFLOOR_H

// Includes
//=========
#include <Engine/EntityComponentSystem/Entity.h>

// Forward Declarations
//=====================

// Class Declaration
//==================

namespace eae6320
{
	class cFloor: public ECS::Entity<cFloor>
	{
	public:
		cFloor();

		virtual void Initialize() override;
	};
}

#endif	// CGOAL_H

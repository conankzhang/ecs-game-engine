/*
	Goal interface.
*/

#ifndef CGOAL_H
#define CGOAL_H

// Includes
//=========
#include <Engine/EntityComponentSystem/Entity.h>

// Forward Declarations
//=====================

// Class Declaration
//==================

namespace eae6320
{
	class cGoal: public ECS::Entity<cGoal>
	{
	public:
		cGoal();

		virtual void Initialize() override;
	};
}

#endif	// CGOAL_H

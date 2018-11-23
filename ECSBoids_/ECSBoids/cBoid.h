/*
	Boid interface.
*/

#ifndef CBOID_H
#define CBOID_H

// Includes
//=========
#include <Engine/EntityComponentSystem/Entity.h>

// Forward Declarations
//=====================

// Class Declaration
//==================

namespace eae6320
{
	class cBoid: public ECS::Entity<cBoid>
	{
	public:
		cBoid();

		virtual void Initialize() override;
	};
}

#endif	// CBOID_H

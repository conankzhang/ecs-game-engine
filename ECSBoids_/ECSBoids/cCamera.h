/*
	Camera interface.
*/

#ifndef CCAMERA_H
#define CCAMERA_H

// Includes
//=========
#include <Engine/EntityComponentSystem/Entity.h>

// Forward Declarations
//=====================

// Class Declaration
//==================

namespace eae6320
{
	class cCamera: public ECS::Entity<cCamera>
	{
	public:
		cCamera();

		virtual void Initialize() override;
	};
}

#endif	// CCAMERA_H

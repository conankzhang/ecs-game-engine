/*
	Input System interface.
*/

#ifndef CINPUTSYSTEM_H
#define CINPUTSYSTEM_H

// Includes
//=========
#include <Engine/EntityComponentSystem/System.h>

// Forward Declarations
//=====================

// Class Declaration
//==================
namespace eae6320
{
	class cInputSystem : public ECS::System<cInputSystem>
	{
	public:

		cInputSystem();
		~cInputSystem();

		void UpdateInput();
	};
}

#endif // CINPUTSYSTEM_H

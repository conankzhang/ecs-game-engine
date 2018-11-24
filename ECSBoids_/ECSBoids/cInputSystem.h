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
namespace eae6320
{
	class cCameraComponent;
}

// Class Declaration
//==================
namespace eae6320
{
	class cInputSystem : public ECS::System<cInputSystem>
	{
	public:

		cInputSystem();
		~cInputSystem();

		virtual void Initialize() override;
		void UpdateInput();

	private:
		cCameraComponent* m_cameraComponent;
	};
}

#endif // CINPUTSYSTEM_H

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
	class cGoalComponent;
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

	protected:
		void HandleFollowToggle();

	private:
		cCameraComponent* m_cameraComponent;
		cGoalComponent* m_goalComponent;
		float m_cameraFollowSpeed;
		bool m_aIsPressed;
		bool m_goalFollowsCamera;
	};
}

#endif // CINPUTSYSTEM_H

/*
	Render System interface.
*/

#ifndef CRENDERSYSTEM_H
#define CRENDERSYSTEM_H

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
	class cRenderSystem : public ECS::System<cRenderSystem>
	{
	public:

		cRenderSystem();
		~cRenderSystem();

		virtual void Initialize() override;
		virtual void Update(float deltaTime) override;

		void SubmitDataToBeRendered(const float i_deltaSystemTime, const float i_deltaTime);

	private:
		cCameraComponent* m_cameraComponent;
	};
}

#endif // CRENDERSYSTEM_H

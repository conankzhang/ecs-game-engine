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

// Class Declaration
//==================
namespace eae6320
{
	class cRenderSystem : public ECS::System<cRenderSystem>
	{
	public:

		cRenderSystem();
		~cRenderSystem();

		virtual void Update(float deltaTime) override;
		virtual void PostUpdate(float deltaTime) override;
	};
}

#endif // CRENDERSYSTEM_H

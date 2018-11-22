/*
	Physics System interface.
*/

#ifndef CPHYSICSSYSTEM_H
#define CPHYSICSSYSTEM_H

// Includes
//=========
#include <Engine/EntityComponentSystem/ECS.h>

// Forward Declarations
//=====================

// Class Declaration
//==================
namespace eae6320
{
	class cPhysicsSystem : public eae6320::ECS::System<cPhysicsSystem>
	{
	public:

		cPhysicsSystem();
		virtual ~cPhysicsSystem();

		virtual void PreUpdate(float deltaTime) override;
		virtual void Update(float deltaTime) override;
		virtual void PostUpdate(float deltaTime) override;
	};
}

#endif // CPHYSICSSYSTEM_H

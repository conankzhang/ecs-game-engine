/*
	Render Component interface.
*/

#ifndef CRENDERCOMPONENT_H
#define CRENDERCOMPONENT_H

// Includes
//=========
#include <Engine/EntityComponentSystem/Component.h>
#include <Engine/Physics/sRigidBodyState.h>

// Forward Declarations
//=====================

namespace eae6320
{
	namespace Graphics
	{
		class cMesh;
		class cEffect;
	}
}

// Class Declaration
//==================
namespace eae6320
{
	class cRenderComponent : public ECS::Component<cRenderComponent>
	{
	public:
		cRenderComponent(const char* const i_prefabPath, Math::sVector i_position, Math::cQuaternion i_orientation);
		~cRenderComponent();

		void LoadPrefab(const char* const i_prefabPath);

		void CleanUp();

		Graphics::cMesh* GetMesh();
		Graphics::cEffect* GetEffect();

		void Update(const float i_deltaTime);
		void SetVelocity(Math::sVector i_velocity);
		Math::sVector GetPosition();
		void SetPosition(Math::sVector i_position);
		Math::cMatrix_transformation GetTransform(const float i_deltaTime);

	protected:
		Graphics::cMesh* m_mesh;
		Graphics::cEffect* m_effect;

		Physics::sRigidBodyState m_rigidBody;
	};
}

#endif // CRENDERCOMPONENT_H

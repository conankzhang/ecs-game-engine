/*
	This class is your specific game
*/

#ifndef EAE6320_CGAMEOBJECT_H
#define EAE6320_CGAMEOBJECT_H

// Includes
//=========
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
	class cGameObject
	{
	public:
		cGameObject(eae6320::Math::sVector i_position, eae6320::Math::cQuaternion i_orientation);
		void CleanUp();
		
		void Update(const float i_elapsedSecondCount_sinceLastUpdate);

		void SetVelocity(eae6320::Math::sVector i_velocity);

		void SetMeshAndEffect(eae6320::Graphics::cMesh* i_mesh, eae6320::Graphics::cEffect* i_effect);
		eae6320::Graphics::cMesh* GetMesh();
		eae6320::Graphics::cEffect* GetEffect();

		eae6320::Math::cMatrix_transformation GetTransform(const float i_elapsedSecondCount_sinceLastSimulationUpdate);
	protected:

		eae6320::Physics::sRigidBodyState m_rigidBody;

		eae6320::Graphics::cMesh* m_mesh;
		eae6320::Graphics::cEffect* m_effect;
	};
}

#endif	// EAE6320_CGAMEOBJECT_H

/*
	Prefab interface.
*/

#ifndef CPREFAB_H
#define CPREFAB_H

// Includes
//=========
#include <Engine/Physics/sRigidBodyState.h>
#include <Engine/Results/Results.h>
#include <Engine/EntityComponentSystem/ECS.h>



// Forward Declarations
//=====================

// Class Declaration
//==================

namespace eae6320
{
	class cPrefab : public ECS::Entity<cPrefab>
	{
	public:
		cPrefab(const char* const i_prefabPath, eae6320::Math::sVector i_position, eae6320::Math::cQuaternion i_orientation);

		static cResult Load( const char* const i_path, char*& o_meshPath, char*& o_effectPath, bool& o_isActive);
		virtual void Initialize() override;

	protected:

		char* m_meshPath;
		char* m_effectPath;
		eae6320::Math::sVector m_position;
		eae6320::Math::cQuaternion m_orientation;
	};
}

#endif	// CPREFAB_H

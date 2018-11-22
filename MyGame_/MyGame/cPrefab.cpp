// Includes
//=========

#include "cPrefab.h"

#include "cMeshComponent.h"
#include "cEffectComponent.h"
#include "cRigidbodyComponent.h"

#include <Engine/Graphics/cEffect.h>
#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Platform/Platform.h>

eae6320::cPrefab::cPrefab(const char* const i_prefabPath, eae6320::Math::sVector i_position, eae6320::Math::cQuaternion i_orientation)
{
	auto result = Results::Success;

	m_position = i_position;
	m_orientation = i_orientation;

	{
		if ( !( result = eae6320::cPrefab::Load("data/Prefabs/object.prefab", m_meshPath, m_effectPath, m_isActive) ) )
		{
			EAE6320_ASSERT( false );
		}
	}
}

eae6320::cResult eae6320::cPrefab::Load(const char* const i_path,  char*& o_meshPath, char*& o_effectPath, bool& o_isActive)
{
	auto result = Results::Success;

	eae6320::Platform::sDataFromFile dataFromFile;
	eae6320::Platform::LoadBinaryFile(i_path, dataFromFile);

	auto currentOffset = reinterpret_cast<uintptr_t>( dataFromFile.data );
	const auto finalOffset = currentOffset + dataFromFile.size;

    o_isActive = *reinterpret_cast<uint8_t*>( currentOffset );

	currentOffset += sizeof( o_isActive );
    const auto meshPathSize = *reinterpret_cast<uint16_t*>( currentOffset );

	currentOffset += sizeof( meshPathSize );
	o_meshPath = reinterpret_cast<char*>( currentOffset );

	currentOffset += meshPathSize * sizeof( char );
	o_effectPath = reinterpret_cast<char*>( currentOffset );

	return result;
}

void eae6320::cPrefab::Initialize()
{
	if (m_meshPath != nullptr)
	{
		AddComponent<cMeshComponent>(m_meshPath);
	}

	if (m_effectPath != nullptr)
	{
		AddComponent<cEffectComponent>(m_effectPath);
	}

	AddComponent<cRigidbodyComponent>(m_position, m_orientation);
}

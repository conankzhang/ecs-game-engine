/*
	Mesh Component implementation.
*/

// Includes
//=========

#include "cMeshComponent.h"

#include <Engine/Graphics/cMesh.h>
#include <Engine/Platform/Platform.h>

// Class Declaration
//==================

eae6320::cMeshComponent::cMeshComponent(const char* const i_meshPath)
{
	auto result = Results::Success;

	if ( !( result =  eae6320::Graphics::cMesh::Load( i_meshPath, m_mesh ) ) )
	{
		EAE6320_ASSERT( false );
	}
}

eae6320::cMeshComponent::~cMeshComponent()
{
	CleanUp();
}

eae6320::Graphics::cMesh* eae6320::cMeshComponent::GetMesh()
{
	return m_mesh;
}

void eae6320::cMeshComponent::CleanUp()
{
	if (m_mesh)
	{
		m_mesh->DecrementReferenceCount();
		m_mesh = nullptr;
	}
}

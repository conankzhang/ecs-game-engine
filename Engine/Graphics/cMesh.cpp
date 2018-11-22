// Includes
//=========

#include "cMesh.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Platform/Platform.h>
#include <Engine/Time/Time.h>
#include <new>
#include "VertexFormats.h"

#include <External/Lua/Includes.h>
#include <iostream>
#include <string>

// Static Data Initialization
//===========================

eae6320::Assets::cManager<eae6320::Graphics::cMesh> eae6320::Graphics::cMesh::s_manager;

// Interface
//==========

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::Graphics::cMesh::Load(const char* const i_path, cMesh*& o_mesh)
{
	auto result = Results::Success;

	cMesh* newMesh = nullptr;

	eae6320::Platform::sDataFromFile dataFromFile;
	eae6320::Platform::LoadBinaryFile(i_path, dataFromFile);

	auto currentOffset = reinterpret_cast<uintptr_t>( dataFromFile.data );
	const auto finalOffset = currentOffset + dataFromFile.size;

    const auto vertexCount = *reinterpret_cast<uint16_t*>( currentOffset );

	currentOffset += sizeof( vertexCount );
	const auto indexCount = *reinterpret_cast<uint16_t*>( currentOffset );

	currentOffset += sizeof( indexCount );
	auto* vertexData = reinterpret_cast<VertexFormats::sMesh*>( currentOffset );

	currentOffset += sizeof( VertexFormats::sMesh ) * vertexCount;
	auto* indexData = reinterpret_cast<uint16_t*>( currentOffset );

	if (vertexCount > 65536)
	{
		EAE6320_ASSERTF( false, "Mesh has too many vertices." );
		Logging::OutputError( "%s has too many vertices. Stopping application. ", i_path );
		result = Results::Failure;
		goto OnExit;
	}

	// Allocate a new mesh 
	{
		newMesh = new (std::nothrow) cMesh();
		if ( !newMesh )
		{
			result = Results::OutOfMemory;
			EAE6320_ASSERTF( false, "Couldn't allocate memory for the mesh %s", i_path );
			Logging::OutputError( "Failed to allocate memory for the mesh %s", i_path );
			return result;
		}
	}
	// Initialize the platform-specific graphics API shader object
	if ( !( result = newMesh->InitializeGeometry( vertexData, indexData, vertexCount, indexCount ) ) )
	{
		EAE6320_ASSERTF( false, "Initialization of new mesh failed" );
		goto OnExit;
	}

OnExit:

	if ( result )
	{
		EAE6320_ASSERT( newMesh );
		o_mesh = newMesh;
	}
	else
	{
		if ( newMesh )
		{
			newMesh->DecrementReferenceCount();
			newMesh = nullptr;
		}
		o_mesh = nullptr;
	}

	return result;
}

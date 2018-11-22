// Includes
//=========

#include "cPrefabBuilder.h"
#include "cPrefabLoader.h"

#include <Tools/AssetBuildLibrary/Functions.h>
#include <Engine/Platform/Platform.h>
#include <Engine/Graphics/VertexFormats.h>
#include <fstream>
#include <iostream>
#include <Engine/Time/Time.h>

// Inherited Implementation
//=========================

// Build
//------

eae6320::cResult eae6320::Assets::cPrefabBuilder::Build( const std::vector<std::string>& i_arguments )
{
	auto result = Results::Success;
	std::string errorMessage;

	std::string meshPath;
	std::string effectPath;
	bool isActive;
	size_t meshPathSize = 0; 

	std::ofstream outFile ( m_path_target, std::ofstream::binary);

	{
		if ( !( result = cPrefabLoader::LoadAsset( m_path_source, meshPath, effectPath, isActive)) )
		{
			EAE6320_ASSERTF( false, "Loading prefab failed" );
			goto OnExit;
		}
	}

	meshPath.insert(0, "data/");
	effectPath.insert(0, "data/");

	// + 1 for null terminator count
	meshPathSize = meshPath.size() + 1;

	// write to outfile
	if (!outFile.is_open())
	{
		goto OnExit;
	}

	outFile.write ( (char *)&isActive, sizeof( uint8_t ) );
	outFile.write ( (char *)&meshPathSize, sizeof( uint16_t ) );
	outFile.write ( meshPath.c_str(), meshPath.size() );
	outFile.put('\0');
	outFile.write ( effectPath.c_str(), effectPath.size());
	outFile.put('\0');

OnExit:
	outFile.close();

	return result;
}

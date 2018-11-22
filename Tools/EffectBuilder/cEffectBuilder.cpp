// Includes
//=========

#include "cEffectBuilder.h"
#include "cEffectLoader.h"

#include <Tools/AssetBuildLibrary/Functions.h>
#include <Engine/Platform/Platform.h>
#include <fstream>
#include <iostream>
#include <Engine/Time/Time.h>
#include <Engine/Graphics/cRenderState.h>

// Inherited Implementation
//=========================

// Build
//------

eae6320::cResult eae6320::Assets::cEffectBuilder::Build( const std::vector<std::string>& i_arguments )
{
	auto result = Results::Success;
	std::string errorMessage;

	std::string vertexShaderPath;
	std::string fragmentShaderPath;
	size_t vertexShaderPathSize = 0; 
	uint8_t renderStateBits = 0;

	std::ofstream outFile ( m_path_target, std::ofstream::binary);

	{
		if ( !( result = cEffectLoader::LoadAsset( m_path_source, vertexShaderPath, fragmentShaderPath, renderStateBits)) )
		{
			EAE6320_ASSERTF( false, "Loading effect failed" );
			goto OnExit;
		}
	}

	vertexShaderPath.insert(0, "data/");
	fragmentShaderPath.insert(0, "data/");

	// + 1 for null terminator count
	vertexShaderPathSize = vertexShaderPath.size() + 1;

	// write to outfile
	if (!outFile.is_open())
	{
		goto OnExit;
	}

	outFile.write ( (char *)&vertexShaderPathSize, sizeof( uint16_t ) );
	outFile.write ( (char *)&renderStateBits, sizeof( uint8_t ) );
	outFile.write ( vertexShaderPath.c_str(), vertexShaderPath.size() );
	outFile.put('\0');
	outFile.write ( fragmentShaderPath.c_str(), fragmentShaderPath.size());
	outFile.put('\0');

OnExit:
	outFile.close();

	return result;
}

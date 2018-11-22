// Includes
//=========

#include "cShader.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Platform/Platform.h>
#include <new>

// Static Data Initialization
//===========================

eae6320::Assets::cManager<eae6320::Graphics::cShader> eae6320::Graphics::cShader::s_manager;

// Interface
//==========

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::Graphics::cShader::Load( const char* const i_path, cShader*& o_shader, const ShaderTypes::eType i_type )
{
	auto result = Results::Success;

	Platform::sDataFromFile dataFromFile;
	cShader* newShader = nullptr;

	// Validate the shader type
	{
		switch ( i_type )
		{
		case ShaderTypes::Vertex:
		case ShaderTypes::Fragment:
			break;
		default:
			result = Results::Failure;
			EAE6320_ASSERTF( false, "Invalid shader type: %i", i_type );
			Logging::OutputError( "An attempt was made to load a shader with an invalid type (%i)", i_type );
			goto OnExit;
		}
	}
	// Load the binary data
	{
		std::string errorMessage;
		if ( !( result = Platform::LoadBinaryFile( i_path, dataFromFile, &errorMessage ) ) )
		{
			EAE6320_ASSERTF( false, errorMessage.c_str() );
			Logging::OutputError( "Failed to load shader from file %s: %s", i_path, errorMessage.c_str() );
			goto OnExit;
		}
	}
	// Allocate a new shader
	{
		newShader = new (std::nothrow) cShader( i_type );
		if ( !newShader )
		{
			result = Results::OutOfMemory;
			EAE6320_ASSERTF( false, "Couldn't allocate memory for the shader %s", i_path );
			Logging::OutputError( "Failed to allocate memory for the shader %s", i_path );
			goto OnExit;
		}
	}
	// Initialize the platform-specific graphics API shader object
	if ( !( result = newShader->Initialize( i_path, dataFromFile ) ) )
	{
		EAE6320_ASSERTF( false, "Initialization of new shader failed" );
		goto OnExit;
	}

OnExit:

	if ( result )
	{
		EAE6320_ASSERT( newShader );
		o_shader = newShader;
	}
	else
	{
		if ( newShader )
		{
			newShader->DecrementReferenceCount();
			newShader = nullptr;
		}
		o_shader = nullptr;
	}
	dataFromFile.Free();

	return result;
}

// Implementation
//===============

// Initialization / Clean Up
//--------------------------
eae6320::Graphics::cShader::cShader( const ShaderTypes::eType i_type )
	:
	m_type( i_type )
{

}

eae6320::Graphics::cShader::~cShader()
{
	CleanUp();
}

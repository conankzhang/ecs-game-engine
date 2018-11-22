// Includes
//=========

#include "../cShader.h"

#include "Includes.h"
#include "../sContext.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Platform/Platform.h>

// Implementation
//===============

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::Graphics::cShader::Initialize( const char* const i_path, const Platform::sDataFromFile& i_loadedShader )
{
	auto* const direct3dDevice = sContext::g_context.direct3dDevice;
	EAE6320_ASSERT( direct3dDevice );

	// Create the vertex shader object
	{
		HRESULT d3dResult;
		ID3D11ClassLinkage* const noInterfaces = nullptr;
		switch ( m_type )
		{
		case ShaderTypes::Vertex:
			d3dResult = direct3dDevice->CreateVertexShader(i_loadedShader.data, i_loadedShader.size, noInterfaces, &m_shaderObject.vertex );
			break;
		case ShaderTypes::Fragment:
			d3dResult = direct3dDevice->CreatePixelShader(i_loadedShader.data, i_loadedShader.size, noInterfaces, &m_shaderObject.fragment );
			break;
		default:
			EAE6320_ASSERT( false );
		}
		if ( SUCCEEDED( d3dResult ) )
		{
			return Results::Success;
		}
		else
		{
			EAE6320_ASSERTF( false, "Shader %s object creation failed (HRESULT %#010x)", i_path, d3dResult );
			eae6320::Logging::OutputError( "Direct3d failed to create the shader %s with HRESULT %#010x", i_path, d3dResult );
			return Results::Failure;
		}
	}
}

eae6320::cResult eae6320::Graphics::cShader::CleanUp()
{
	auto result = Results::Success;

	switch ( m_type )
	{
	case ShaderTypes::Vertex:
		if ( m_shaderObject.vertex )
		{
			m_shaderObject.vertex->Release();
			m_shaderObject.vertex = nullptr;
		}
		break;
	case ShaderTypes::Fragment:
		if ( m_shaderObject.fragment )
		{
			m_shaderObject.fragment->Release();
			m_shaderObject.fragment = nullptr;
		}
		break;
	default:
		EAE6320_ASSERT( false );
	}

	return result;
}

// Includes
//=========

#include "../cShaderBuilder.h"

#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <Engine/Platform/Platform.h>
#include <iostream>
#include <Tools/AssetBuildLibrary/Functions.h>

// Helper Class Declaration
//=========================

namespace
{
	class cIncludeHelper : public ID3DInclude
	{
		// Interface
		//----------

	public:

		eae6320::cResult Initialize();
		cIncludeHelper( const std::string& i_shaderSourcePath );

		// Inherited Interface
		//--------------------

	private:

		virtual HRESULT Open( D3D_INCLUDE_TYPE i_includeType, const char* i_path, const void* i_parentData,
			const void** o_dataFromFile, unsigned int* o_dataSize ) override;
		virtual HRESULT Close( const void* i_data ) override;

		// Data
		//-----

	private:

		const std::string m_shaderSourcePath;
		std::string m_shaderSourceDirectory;
		std::string m_engineSourceContentDirectory;
		std::string m_gameSourceContentDirectory;
	};
}

// Interface
//==========

// Build
//------

eae6320::cResult eae6320::Assets::cShaderBuilder::Build( const Graphics::ShaderTypes::eType i_shaderType, const std::vector<std::string>& i_arguments )
{
	auto result = Results::Success;

	Platform::sDataFromFile dataFromFile;
	ID3DBlob* compiledCode = nullptr;

	// Load the source code
	{
		std::string errorMessage;
		if ( !( result = Platform::LoadBinaryFile( m_path_source, dataFromFile, &errorMessage ) ) )
		{
			OutputErrorMessageWithFileInfo( m_path_source, "Failed to load shader source file: %s", errorMessage.c_str() );
			goto OnExit;
		}
	}
	// Compile it
	{
		const D3D_SHADER_MACRO defines[] =
		{
			{ "EAE6320_PLATFORM_D3D" },
			{}
		};
		cIncludeHelper includeHelper( m_path_source );
		if ( !( result = includeHelper.Initialize() ) )
		{
			goto OnExit;
		}
		constexpr auto entryPoint = "main";
		const char* targetProfile;
		switch ( i_shaderType )
		{
		case Graphics::ShaderTypes::Vertex:
			targetProfile = "vs_4_0";
			break;
		case Graphics::ShaderTypes::Fragment:
			targetProfile = "ps_4_0";
			break;
		}
		constexpr unsigned int compileConstants = 0
#ifdef EAE6320_GRAPHICS_AREDEBUGSHADERSENABLED
			// Include debugging information
			| D3DCOMPILE_DEBUG
			// Disable optimizations
			| D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_OPTIMIZATION_LEVEL0
#else
			// Optimize as much as possible
			| D3DCOMPILE_OPTIMIZATION_LEVEL3
#endif
			// Treat warnings as errors
			| D3DCOMPILE_WARNINGS_ARE_ERRORS
			;
		constexpr unsigned int notAnFxFile = 0;
		ID3DBlob* errorMessages = nullptr;
		const auto d3dResult = D3DCompile( dataFromFile.data, dataFromFile.size, m_path_source, defines, &includeHelper, entryPoint,
			targetProfile, compileConstants, notAnFxFile, &compiledCode, &errorMessages );
		if ( errorMessages )
		{
			std::cerr << static_cast<char*>( errorMessages->GetBufferPointer() );
			errorMessages->Release();
			errorMessages = nullptr;
		}
		else if ( FAILED( d3dResult ) )
		{
			OutputErrorMessageWithFileInfo( m_path_source, "Shader compiling failed for unknown reason" );
		}
		if ( FAILED( d3dResult ) )
		{
			result = Results::Failure;
			goto OnExit;
		}
	}
	// Write the compiled shader to disk
	{
		std::string errorMessage;
		if ( !( result = eae6320::Platform::WriteBinaryFile( m_path_target, compiledCode->GetBufferPointer(), compiledCode->GetBufferSize(), &errorMessage ) ) )
		{
			eae6320::Assets::OutputErrorMessageWithFileInfo( m_path_source, errorMessage.c_str() );
		}
	}

OnExit:

	if ( compiledCode )
	{
		compiledCode->Release();
		compiledCode = nullptr;
	}
	dataFromFile.Free();

	return result;
}

// Helper Class Definition
//========================

namespace
{
	// Interface
	//----------

	eae6320::cResult cIncludeHelper::Initialize()
	{
		auto result = eae6320::Results::Success;

		// Calculate the directory of the source file
		{
			const auto pos_lastSlash = m_shaderSourcePath.find_last_of( "/\\" );
			if ( pos_lastSlash != m_shaderSourcePath.npos )
			{
				m_shaderSourceDirectory = m_shaderSourcePath.substr( 0, pos_lastSlash + 1 );
			}
			else
			{
				m_shaderSourceDirectory = "";
			}
		}
		// Get the content directories to use as #include search paths
		{
			std::string errorMessage;

			// EngineSourceContentDir
			{
				if ( !( result = eae6320::Platform::GetEnvironmentVariable(
					"EngineSourceContentDir", m_engineSourceContentDirectory, &errorMessage ) ) )
				{
					eae6320::Assets::OutputErrorMessage( "Failed to get the engine's source content directory: %s", errorMessage.c_str() );
					goto OnExit;
				}
			}
			// GameSourceContentDir
			{
				if ( !( result = eae6320::Platform::GetEnvironmentVariable(
					"GameSourceContentDir", m_gameSourceContentDirectory, &errorMessage ) ) )
				{
					eae6320::Assets::OutputErrorMessage( "Failed to get the game's source content directory: %s", errorMessage.c_str() );
					goto OnExit;
				}
			}
		}

	OnExit:

		return result;
	}

	cIncludeHelper::cIncludeHelper( const std::string& i_shaderSourcePath )
		:
		m_shaderSourcePath( i_shaderSourcePath )
	{

	}

	// Inherited Interface
	//--------------------

	HRESULT cIncludeHelper::Open( D3D_INCLUDE_TYPE i_includeType, const char* i_path, const void* i_parentData,
		const void** o_dataFromFile, unsigned int* o_dataSize )
	{
		auto result = S_OK;

		eae6320::Platform::sDataFromFile dataFromFile;

		EAE6320_ASSERT( o_dataFromFile );
		EAE6320_ASSERT( o_dataSize );

		// Calculate the path to open
		std::string pathToOpen;
		{
			// BUG WARNING!
			// The code below assumes that the #include path is relative,
			// and it will not work correctly for absolute paths
			switch ( i_includeType )
			{
			case D3D_INCLUDE_LOCAL:
				{
					pathToOpen = m_shaderSourceDirectory + i_path;
					if ( !eae6320::Platform::DoesFileExist( pathToOpen.c_str() ) )
					{
						result = ERROR_PATH_NOT_FOUND;
						eae6320::Assets::OutputErrorMessageWithFileInfo( m_shaderSourcePath.c_str(),
							"The file %s doesn't exist (from the requested #include path \"%s\"", pathToOpen.c_str(), i_path );
						goto OnExit;
					}
				}
				break;
			case D3D_INCLUDE_SYSTEM:
				{
					// Files in the game source content directory take precedence over those in the engine source content directory
					pathToOpen = m_gameSourceContentDirectory + i_path;
					if ( !eae6320::Platform::DoesFileExist( pathToOpen.c_str() ) )
					{
						pathToOpen = m_engineSourceContentDirectory + i_path;
						if ( !eae6320::Platform::DoesFileExist( pathToOpen.c_str() ) )
						{
							result = ERROR_PATH_NOT_FOUND;
							eae6320::Assets::OutputErrorMessageWithFileInfo( m_shaderSourcePath.c_str(),
								"The file %s doesn't exist (from the requested #include path \"%s\"", pathToOpen.c_str(), i_path );
							goto OnExit;
						}
					}
				}
				break;
			}
		}

		{
			std::string errorMessage;
			const auto localResult = eae6320::Platform::LoadBinaryFile( pathToOpen.c_str(), dataFromFile, &errorMessage );
			if ( localResult )
			{
				*o_dataFromFile = dataFromFile.data;
				EAE6320_ASSERT( dataFromFile.size < ( uint64_t( 1 ) << ( sizeof( *o_dataSize ) * 8 ) ) );
				*o_dataSize = static_cast<unsigned int>( dataFromFile.size );
			}
			else
			{
				eae6320::Assets::OutputErrorMessageWithFileInfo( __FILE__, __LINE__,
					"Failed to open requested #include path: %s",
					errorMessage.c_str() );
				goto OnExit;
			}
		}

	OnExit:

		if ( FAILED( result ) )
		{
			dataFromFile.Free();
			*o_dataFromFile = nullptr;
			*o_dataSize = 0;
		}

		return result;
	}

	HRESULT cIncludeHelper::Close( const void* i_data )
	{
		EAE6320_ASSERT( i_data );

		// This data should be from the Open() function:
		// Assign it bad to the platform data structure
		// so that it can be deallocated consistently
		eae6320::Platform::sDataFromFile dataFromFile;
		dataFromFile.data = const_cast<void*>( i_data );
		dataFromFile.Free();

		return S_OK;
	}
}

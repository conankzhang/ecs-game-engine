/*
	This class loads meshes
*/

#ifndef EAE6320_CEFFECTLOADER_H
#define EAE6320_CEFFECTLOADER_H

// Includes
//=========

#include <Engine/Graphics/Configuration.h>
#include <Engine/Graphics/cEffect.h>
#include <Engine/Graphics/cMesh.h>

// Forward Declarations
//=====================

namespace eae6320
{
	namespace Platform
	{
		struct sDataFromFile;
	}

	namespace Graphics
	{
		namespace VertexFormats
		{
			struct sMesh;
		}
	}
}

// Class Declaration
//==================

namespace eae6320
{
	namespace Assets
	{
		class cEffectLoader
		{
			// Inherited Implementation
			//=========================
		public:
			static eae6320::cResult LoadAsset( const char* const i_path, std::string& io_vertexShaderPath, std::string& io_fragmentShaderPath, uint8_t& io_renderStateBits );


		private:

			// Build
			//------


			// Implementation
			//===============
			static eae6320::cResult LoadTableValues( lua_State& io_luaState, std::string& io_vertexShaderPath, std::string& io_fragmentShaderPath, uint8_t& io_renderStateBits);

		};
	}
}

#endif	// EAE6320_CMESHLOADER_H

/*
	This class loads prefabs 
*/

#ifndef EAE6320_CPREFABLOADER_H
#define EAE6320_CPREFABLOADER_H

// Includes
//=========

#include <Engine/Graphics/Configuration.h>
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
		class cPrefabLoader
		{
			// Inherited Implementation
			//=========================
		public:
			static eae6320::cResult LoadAsset( const char* const i_path, std::string& io_meshPath, std::string& io_effectPath, bool& io_isActive);

		private:

			// Build
			//------

			// Implementation
			//===============
			static eae6320::cResult LoadTableValues( lua_State& io_luaState, std::string& io_meshPath, std::string& io_effectPath, bool& io_isActive);

			static eae6320::cResult LoadTableValues_isActive( lua_State& io_luaState, bool& io_isActive);
			static eae6320::cResult LoadTableValues_componentArray( lua_State& io_luaState, std::string& io_meshPath, std::string& io_effectPath);
			static eae6320::cResult LoadTableValues_components( lua_State& io_luaState, std::string& io_meshPath, std::string& io_effectPath);
			static eae6320::cResult LoadTableValues_componentValues( lua_State& io_luaState, std::string& io_meshPath, std::string& io_effectPath);
		};
	}
}

#endif	// EAE6320_CPREFABLOADER_H

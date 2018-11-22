/*
	This class loads meshes
*/

#ifndef EAE6320_CMESHLOADER_H
#define EAE6320_CMESHLOADER_H

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
		class cMeshLoader
		{
			// Inherited Implementation
			//=========================
		public:
			static eae6320::cResult LoadAsset( const char* const i_path, std::vector<Graphics::VertexFormats::sMesh>& io_vertexData, std::vector<uint16_t>& io_indexData );


		private:

			// Build
			//------


			// Implementation
			//===============
			static eae6320::cResult LoadTableValues( lua_State& io_luaState, std::vector<Graphics::VertexFormats::sMesh>& io_vertexData, std::vector<uint16_t>& io_indexData );

			static eae6320::cResult LoadTableValues_vertexArray(lua_State& io_luaState, std::vector<Graphics::VertexFormats::sMesh>& io_vertexData);
			static eae6320::cResult LoadTableValues_vertexArray_vertices( lua_State& io_luaState, std::vector<Graphics::VertexFormats::sMesh>& io_vertexData);
			static eae6320::cResult LoadTableValues_vertexArray_vertices_position( lua_State& io_luaState, std::vector<Graphics::VertexFormats::sMesh>& io_vertexData);
			static eae6320::cResult LoadTableValues_vertexArray_vertices_position_values( lua_State& io_luaState, std::vector<Graphics::VertexFormats::sMesh>& io_vertexData);

			static eae6320::cResult LoadTableValues_indexArray(lua_State& io_luaState, std::vector<uint16_t>& io_indexData);
			static eae6320::cResult LoadTableValues_indexArray_indices( lua_State& io_luaState, std::vector<uint16_t>& io_indexData);


		};
	}
}

#endif	// EAE6320_CMESHLOADER_H

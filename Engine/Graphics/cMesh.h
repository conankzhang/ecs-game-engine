/*
	A shader is a program that runs on the GPU (graphics hardware)

	This class encapsulates a platform-specific graphics API objec
	that can be loaded and refered to on the CPU.
*/

#ifndef EAE6320_GRAPHICS_CMESH_H
#define EAE6320_GRAPHICS_CMESH_H

// Includes
//=========

#include "Configuration.h"

#include <Engine/Assets/ReferenceCountedAssets.h>

#include <cstdint>
#include <Engine/Assets/ReferenceCountedAssets.h>
#include <Engine/Assets/cHandle.h>
#include <Engine/Assets/cManager.h>
#include <Engine/Results/Results.h>

#ifdef EAE6320_PLATFORM_GL
	#include "OpenGL/Includes.h"
#endif

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

struct lua_State;

#ifdef EAE6320_PLATFORM_D3D
	struct ID3D11Buffer;
	struct ID3D11InputLayout;
#endif
// Class Declaration
//==================

namespace eae6320
{
	namespace Graphics
	{
		class cMesh
		{
			// Interface
			//==========

		public:
			// Assets
			//-------

			using Handle = Assets::cHandle<cMesh>;
			static Assets::cManager<cMesh> s_manager;

			// Initialization / Clean Up
			//--------------------------
			static cResult Load( eae6320::Graphics::cMesh*& o_mesh, std::vector<VertexFormats::sMesh> i_vertexData, std::vector<uint16_t> i_indexData );
			static cResult Load( const char* const i_path, cMesh*& o_mesh );

			void RenderFrame();

			EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(cMesh);
			EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS();
			
			// Data
			//=====

		public:
#if defined( EAE6320_PLATFORM_D3D )
			// A vertex buffer holds the data for each vertex
			ID3D11Buffer* m_vertexBuffer = nullptr;
			// D3D has an "input layout" object that associates the layout of the vertex format struct
			// with the input from a vertex shader
			ID3D11InputLayout* m_vertexInputLayout = nullptr;
			// An index buffer holds the index for each vertex to draw
			ID3D11Buffer* m_indexBuffer = nullptr;
#elif defined( EAE6320_PLATFORM_GL )
			// A vertex buffer holds the data for each vertex
			GLuint m_vertexBufferId = 0;
			// A vertex array encapsulates the vertex data as well as the vertex input layout
			GLuint m_vertexArrayId = 0;
			// An index buffer holds the index for each vertex to draw
			GLuint m_indexBufferId = 0;
			// An index array encapsulates the index data as well as the index input layout
			GLuint m_indexArrayId = 0;
#endif
			EAE6320_ASSETS_DECLAREREFERENCECOUNT();

			// Implementation
			//===============
		private:
			size_t m_indexCountToRender = 0;
			cResult InitializeGeometry( VertexFormats::sMesh* i_vertexData, uint16_t* i_indexData, uint16_t i_vertexCount, uint16_t i_indexCount);
			cResult CleanUp();

			cMesh();
			~cMesh();
		};
	}
}

#endif	// EAE6320_GRAPHICS_CMESH_H

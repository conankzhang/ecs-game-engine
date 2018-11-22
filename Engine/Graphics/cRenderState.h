/*
	"Render state" is a generic term for fixed-function state
	that can be configured on the GPU

	"Fixed-function" means that it isn't programmable like a shader,
	but instead has a pre-determined set of behaviors that can be configured.
*/

#ifndef EAE6320_GRAPHICS_CRENDERSTATE_H
#define EAE6320_GRAPHICS_CRENDERSTATE_H

// Includes
//=========

#include "Configuration.h"

#include <cstdint>
#include <Engine/Results/Results.h>

// Render State Bits
//==================

namespace eae6320
{
	namespace Graphics
	{
		namespace RenderStates
		{
			// The values below are used as bit masks
			enum eRenderState : uint8_t
			{
				// Set to enable alpha transparency
				AlphaTransparency = 1 << 0,
				// Set to enable both depth testing and writing to the depth buffer
				DepthBuffering = 1 << 1,
				// Set to draw triangles regardless of whether they are front-facing or back-facing
				// (set to disable culling)
				DrawBothTriangleSides = 1 << 2
			};

			// Alpha Transparency
			constexpr bool IsAlphaTransparencyEnabled( const uint8_t i_renderStateBits );
			void EnableAlphaTransparency( uint8_t& io_renderStateBits );
			void DisableAlphaTransparency( uint8_t& io_renderStateBits );
			// Depth Buffering
			constexpr bool IsDepthBufferingEnabled( const uint8_t i_renderStateBits );
			void EnableDepthBuffering( uint8_t& io_renderStateBits );
			void DisableDepthBuffering( uint8_t& io_renderStateBits );
			// Draw Both Triangle Sides
			constexpr bool ShouldBothTriangleSidesBeDrawn( const uint8_t i_renderStateBits );
			void EnableDrawingBothTriangleSides( uint8_t& io_renderStateBits );
			void DisableDrawingBothTriangleSides( uint8_t& io_renderStateBits );
		}
	}
}

// Forward Declarations
//=====================

#if defined( EAE6320_PLATFORM_D3D )
	struct ID3D11BlendState;
	struct ID3D11DepthStencilState;
	struct ID3D11RasterizerState;
#endif

// Class Declaration
//==================

namespace eae6320
{
	namespace Graphics
	{
		class cRenderState
		{
			// Interface
			//==========

		public:

			// Render
			//-------

			void Bind() const;

			// Access
			//-------

			bool IsAlphaTransparencyEnabled() const;
			bool IsDepthBufferingEnabled() const;
			bool ShouldBothTriangleSidesBeDrawn() const;

			uint8_t GetRenderStateBits() const;

			// Initialization / Clean Up
			//--------------------------

			// The input parameter is a concatenation of RenderStates::eRenderState bits
			// which define which render states should be enabled
			cResult Initialize( const uint8_t i_renderStateBits );
			cResult CleanUp();

			cRenderState() = default;
			~cRenderState();

			// Data
			//=====

		private:

#if defined( EAE6320_PLATFORM_D3D )
			ID3D11BlendState* m_blendState = nullptr;
			ID3D11DepthStencilState* m_depthStencilState = nullptr;
			ID3D11RasterizerState* m_rasterizerState = nullptr;
#endif

			// A reasonable default could be set,
			// but choosing a recognizable impossible value can help in debugging
			// (if a cRenderState ever has these bits it shows that it wasn't initialized)
			uint8_t m_bits = 0xff;

			// Implementation
			//===============

			// Initialization / Clean Up
			//--------------------------

			cResult InitializeFromBits();
		};
	}
}

#include "cRenderState.inl"

#endif	// EAE6320_GRAPHICS_CRENDERSTATE_H

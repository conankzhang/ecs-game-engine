/*
	A shader is a program that runs on the GPU (graphics hardware)

	This class encapsulates a platform-specific graphics API objec
	that can be loaded and refered to on the CPU.
*/

#ifndef EAE6320_GRAPHICS_CEFFECT_H
#define EAE6320_GRAPHICS_CEFFECT_H

// Includes
//=========

#include "Configuration.h"

#include <Engine/Assets/ReferenceCountedAssets.h>

#include <cstdint>
#include <Engine/Assets/ReferenceCountedAssets.h>
#include <Engine/Assets/cHandle.h>
#include <Engine/Assets/cManager.h>
#include "cShader.h"
#include "cRenderState.h"
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
}

// Class Declaration
//==================

namespace eae6320
{
	namespace Graphics
	{
		class cEffect
		{
			// Interface
			//==========

		public:
			// Assets
			//-------

			using Handle = Assets::cHandle<cEffect>;
			static Assets::cManager<cEffect> s_manager;

			// Initialization / Clean Up
			//--------------------------
			static cResult Load(eae6320::Graphics::cEffect*& o_effect, const char i_effectPath[]);

			void RenderFrame();

			EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(cEffect);
			EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS();
			
			// Data
			//=====

			// In this initial assignment the Graphics system needs access to the shader API objects.
			// In future assignments you can make the data private and decide who needs access
			// and the best way to provide it.
		public:

			eae6320::Graphics::cShader::Handle m_vertexShader;
			eae6320::Graphics::cShader::Handle m_fragmentShader;

			eae6320::Graphics::cRenderState m_renderState;

#if defined( EAE6320_PLATFORM_GL )
			GLuint m_programId = 0;
#endif
			EAE6320_ASSETS_DECLAREREFERENCECOUNT();

			// Implementation
			//===============

		private:

			// Initialization / Clean Up
			//--------------------------
			eae6320::cResult InitializeShadingData(const char i_vertexShaderPath[], const char i_fragmentShaderPath[], const uint8_t i_renderStateBits);
			eae6320::cResult CleanUp();

			cEffect();
			~cEffect();
		};
	}
}

#endif	// EAE6320_GRAPHICS_CEFFECT_H

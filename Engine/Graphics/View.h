/*
	A shader is a program that runs on the GPU (graphics hardware)

	This class encapsulates a platform-specific graphics API objec
	that can be loaded and refered to on the CPU.
*/

#ifndef EAE6320_GRAPHICS_VIEW_H
#define EAE6320_GRAPHICS_VIEW_H

// Includes
//=========

#include "Configuration.h"

#include <Engine/Assets/ReferenceCountedAssets.h>

#include <cstdint>
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
		struct sInitializationParameters;
	}
}

// Class Declaration
//==================

namespace eae6320
{
	namespace Graphics
	{
		class View 
		{
			// Interface
			//==========

		public:
			// Assets
			//-------

			// Initialization / Clean Up
			//--------------------------
			eae6320::cResult InitializeViews( const eae6320::Graphics::sInitializationParameters& i_initializationParameters );
			void Swap();
			void CleanUp();
			void RenderFrame();
			void ClearColor( float i_red, float i_green, float i_blue, float i_alpha );

			// Data
			//=====

			// In this initial assignment the Graphics system needs access to the shader API objects.
			// In future assignments you can make the data private and decide who needs access
			// and the best way to provide it.
		public:

			// Implementation
			//===============

		private:

			// Initialization / Clean Up
			//--------------------------
		};
	}
}

#endif	// EAE6320_GRAPHICS_VIEW_H

// Includes
//=========

#include "../View.h"

#include "Includes.h"
#include "../sContext.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Platform/Platform.h>

// Implementation
//===============

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::Graphics::View::InitializeViews( const sInitializationParameters& i_initializationParameters )
{
	return eae6320::Results::Success;
}

void eae6320::Graphics::View::CleanUp()
{
	return;
}

void eae6320::Graphics::View::Swap()
{
	// Everything has been drawn to the "back buffer", which is just an image in memory.
	// In order to display it the contents of the back buffer must be "presented"
	// (or "swapped" with the "front buffer")
	{
		const auto deviceContext = sContext::g_context.deviceContext;
		EAE6320_ASSERT( deviceContext != NULL );

		const auto glResult = SwapBuffers( deviceContext );
		EAE6320_ASSERT( glResult != FALSE );
	}
}

void eae6320::Graphics::View::ClearColor(float i_red, float i_green, float i_blue, float i_alpha)
{
	// Every frame an entirely new image will be created.
	// Before drawing anything the previous image will be erased
	// by "clearing" the image buffer (filling it with a solid color)
	{
		// Black is usually used
		{
			glClearColor( i_red, i_green, i_blue, i_alpha );
			EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
		}
		{
			constexpr GLbitfield clearColor = GL_COLOR_BUFFER_BIT;
			glClear( clearColor );
			EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
		}
	}

	// In addition to the color buffer there is also a hidden image called the "depth buffer"
	// which is used to make it less important which order draw calls are made.
	// It must also be "cleared" every frame just like the visible color buffer.
	{
		{
			glDepthMask( GL_TRUE );
			EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
			constexpr GLclampd clearToFarDepth = 1.0;
			glClearDepth( clearToFarDepth );
			EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
		}
		{
			constexpr GLbitfield clearDepth = GL_DEPTH_BUFFER_BIT;
			glClear( clearDepth );
			EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
		}
	}
}

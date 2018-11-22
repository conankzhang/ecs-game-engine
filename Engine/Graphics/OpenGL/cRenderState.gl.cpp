// Includes
//=========

#include "../cRenderState.h"

#include "Includes.h"

#include <Engine/Asserts/Asserts.h>

// Interface
//==========

// Render
//-------

void eae6320::Graphics::cRenderState::Bind() const
{
	// Alpha Transparency
	if ( IsAlphaTransparencyEnabled() )
	{
		glEnable( GL_BLEND );
		EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
		// result = ( source * source.a ) + ( destination * ( 1 - source.a ) )
		glBlendEquation( GL_FUNC_ADD );
		EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
	}
	else
	{
		glDisable( GL_BLEND );
		EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
	}
	// Depth Buffering
	if ( IsDepthBufferingEnabled() )
	{
		// The new fragment becomes a pixel if its depth is less than what has previously been written
		glEnable( GL_DEPTH_TEST );
		EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
		glDepthFunc( GL_LESS );
		EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
		// Write to the depth buffer
		glDepthMask( GL_TRUE );
		EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
	}
	else
	{
		// Don't test the depth buffer
		glDisable( GL_DEPTH_TEST );
		EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
		// Don't write to the depth buffer
		glDepthMask( GL_FALSE );
		EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
	}
	// Draw Both Triangle Sides
	if ( ShouldBothTriangleSidesBeDrawn() )
	{
		// Don't cull any triangles
		glDisable( GL_CULL_FACE );
		EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
	}
	else
	{
		// Cull triangles that are facing backwards
		glEnable( GL_CULL_FACE );
		EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
		// Triangles use right-handed winding order
		// (opposite from Direct3D)
		glFrontFace( GL_CCW );
		EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
	}
}

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::Graphics::cRenderState::CleanUp()
{
	return Results::Success;
}

// Implementation
//===============

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::Graphics::cRenderState::InitializeFromBits()
{
	// OpenGL uses the bits directly at binding time
	return Results::Success;
}

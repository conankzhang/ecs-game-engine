// Includes
//=========

#include "cRenderState.h"

// Interface
//==========

// Access
//-------

bool eae6320::Graphics::cRenderState::IsAlphaTransparencyEnabled() const
{
	return RenderStates::IsAlphaTransparencyEnabled( m_bits );
}

bool eae6320::Graphics::cRenderState::IsDepthBufferingEnabled() const
{
	return RenderStates::IsDepthBufferingEnabled( m_bits );
}

bool eae6320::Graphics::cRenderState::ShouldBothTriangleSidesBeDrawn() const
{
	return RenderStates::ShouldBothTriangleSidesBeDrawn( m_bits );
}

uint8_t eae6320::Graphics::cRenderState::GetRenderStateBits() const
{
	return m_bits;
}

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::Graphics::cRenderState::Initialize( const uint8_t i_renderStateBits )
{
	m_bits = i_renderStateBits;
	return InitializeFromBits();
}

eae6320::Graphics::cRenderState::~cRenderState()
{
	CleanUp();
}

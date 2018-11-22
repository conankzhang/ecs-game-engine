/*
	Effect Component implementation.
*/

// Includes
//=========

#include "cEffectComponent.h"

#include <Engine/Graphics/cEffect.h>
#include <Engine/Platform/Platform.h>

// Class Declaration
//==================

eae6320::cEffectComponent::cEffectComponent(const char* const i_effectPath)
{
	auto result = Results::Success;

	if ( !( result =  eae6320::Graphics::cEffect::Load( m_effect, i_effectPath ) ) )
	{
		EAE6320_ASSERT( false );
	}
}

eae6320::cEffectComponent::~cEffectComponent()
{
	CleanUp();
}

eae6320::Graphics::cEffect* eae6320::cEffectComponent::GetEffect()
{
	return m_effect;
}

void eae6320::cEffectComponent::CleanUp()
{
	if (m_effect)
	{
		m_effect->DecrementReferenceCount();
		m_effect = nullptr;
	}
}

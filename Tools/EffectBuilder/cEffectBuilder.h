/*
	This class builds effects 
*/

#ifndef EAE6320_CEFFECTBUILDER_H
#define EAE6320_CEFFECTBUILDER_H

// Includes
//=========

#include <Tools/AssetBuildLibrary/cbBuilder.h>

#include <Engine/Graphics/Configuration.h>
#include <Engine/Graphics/cEffect.h>

// Class Declaration
//==================

namespace eae6320
{
	namespace Assets
	{
		class cEffectBuilder : public cbBuilder
		{
			// Inherited Implementation
			//=========================

		private:

			// Build
			//------

			virtual cResult Build( const std::vector<std::string>& i_arguments ) override;

			// Implementation
			//===============
		};
	}
}

#endif	// EAE6320_CEFFECTBUILDER_H

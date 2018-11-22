/*
	This class builds prefabs 
*/

#ifndef EAE6320_CPREFABBUILDER_H
#define EAE6320_CPREFABBUILDER_H

// Includes
//=========

#include <Tools/AssetBuildLibrary/cbBuilder.h>

#include <Engine/Graphics/Configuration.h>

// Class Declaration
//==================

namespace eae6320
{
	namespace Assets
	{
		class cPrefabBuilder : public cbBuilder
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

#endif	// EAE6320_CPREFABBUILDER_H

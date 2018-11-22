/*
	This class builds shaders
*/

#ifndef EAE6320_CMESHBUILDER_H
#define EAE6320_CMESHBUILDER_H

// Includes
//=========

#include <Tools/AssetBuildLibrary/cbBuilder.h>

#include <Engine/Graphics/Configuration.h>
#include <Engine/Graphics/cMesh.h>

// Class Declaration
//==================

namespace eae6320
{
	namespace Assets
	{
		class cMeshBuilder : public cbBuilder
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

#endif	// EAE6320_CMESHBUILDER_H

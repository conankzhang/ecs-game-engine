/*
	This class is for cameras.
*/

#ifndef EAE6320_CCAMERA_H
#define EAE6320_CCAMERA_H

// Includes
//=========
#include "cGameObject.h"

// Forward Declarations
//=====================

namespace eae6320
{
	namespace Graphics
	{
		class cMesh;
		class cEffect;
	}
}

// Class Declaration
//==================

namespace eae6320
{
	class cCamera : public cGameObject 
	{
	public:
		cCamera(eae6320::Math::sVector i_position, eae6320::Math::cQuaternion i_orientation);

		eae6320::Math::cMatrix_transformation GetWorldToCameraTransform(const float i_elapsedSecondCount_sinceLastSimulationUpdate);
		eae6320::Math::cMatrix_transformation GetCameraToProjectedTransform();

	private:
		const float m_verticalFieldOfView_inRadians = 45.0f;
		const float m_aspectRatio = 1.0f;
		const float m_z_nearPlane = 0.1f;
		const float m_z_farPlane = 100.0f;
	};
}

#endif	// EAE6320_CCAMERA_H

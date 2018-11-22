// Includes
//=========

#include "cCamera.h"
#include <Engine/Math/cMatrix_transformation.h>

eae6320::cCamera::cCamera(eae6320::Math::sVector i_position, eae6320::Math::cQuaternion i_orientation)
	:cGameObject(i_position, i_orientation)
{

}

eae6320::Math::cMatrix_transformation eae6320::cCamera::GetWorldToCameraTransform(const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	return eae6320::Math::cMatrix_transformation::CreateWorldToCameraTransform(eae6320::Math::cMatrix_transformation(m_rigidBody.PredictFutureOrientation(i_elapsedSecondCount_sinceLastSimulationUpdate), m_rigidBody.PredictFuturePosition(i_elapsedSecondCount_sinceLastSimulationUpdate)));
}

eae6320::Math::cMatrix_transformation eae6320::cCamera::GetCameraToProjectedTransform()
{
	return eae6320::Math::cMatrix_transformation::CreateCameraToProjectedTransform_perspective(m_verticalFieldOfView_inRadians, m_aspectRatio, m_z_nearPlane, m_z_farPlane);
}

// Includes
//=========

#include "cGameObject.h"

#include <Engine/Graphics/cEffect.h>
#include <Engine/Graphics/cMesh.h>
#include <Engine/Math/cMatrix_transformation.h>

eae6320::cGameObject::cGameObject(eae6320::Math::sVector i_position, eae6320::Math::cQuaternion i_orientation)
{
	m_rigidBody.position = i_position;
	m_rigidBody.orientation = i_orientation;
}

void eae6320::cGameObject::CleanUp()
{
	if (m_effect)
	{
		m_effect->DecrementReferenceCount();
		m_effect = nullptr;
	}

	if (m_mesh)
	{
		m_mesh->DecrementReferenceCount();
		m_mesh = nullptr;
	}
}

void eae6320::cGameObject::Update(const float i_elapsedSecondCount_sinceLastUpdate)
{
	m_rigidBody.Update(i_elapsedSecondCount_sinceLastUpdate);
}

void eae6320::cGameObject::SetVelocity(eae6320::Math::sVector i_velocity)
{
	m_rigidBody.velocity = i_velocity;
}

void eae6320::cGameObject::SetMeshAndEffect(eae6320::Graphics::cMesh* i_mesh, eae6320::Graphics::cEffect* i_effect)
{
	m_mesh = i_mesh;
	m_effect = i_effect;
}

eae6320::Graphics::cMesh* eae6320::cGameObject::GetMesh()
{
	return m_mesh;
}

eae6320::Graphics::cEffect* eae6320::cGameObject::GetEffect()
{
	return m_effect;
}

eae6320::Math::cMatrix_transformation eae6320::cGameObject::GetTransform(const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	return eae6320::Math::cMatrix_transformation(m_rigidBody.PredictFutureOrientation(i_elapsedSecondCount_sinceLastSimulationUpdate), m_rigidBody.PredictFuturePosition(i_elapsedSecondCount_sinceLastSimulationUpdate));
}

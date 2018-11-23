// Includes
//=========

#include "cRenderComponent.h"

#include <Engine/Graphics/cMesh.h>
#include <Engine/Graphics/cEffect.h>
#include <Engine/Math/cMatrix_transformation.h>

eae6320::cRenderComponent::cRenderComponent(const char* const i_meshPath, const char* const i_effectPath, Math::sVector i_position, Math::cQuaternion i_orientation)
{
	Graphics::cMesh::Load( i_meshPath, m_mesh );
	Graphics::cEffect::Load(m_effect, i_effectPath);

	m_rigidBody.position = i_position;
	m_rigidBody.orientation = i_orientation;
}

// Initialization / Clean Up
//--------------------------

eae6320::cRenderComponent::~cRenderComponent()
{
	CleanUp();
}

void eae6320::cRenderComponent::CleanUp()
{
	if (m_mesh)
	{
		m_mesh->DecrementReferenceCount();
		m_mesh = nullptr;
	}

	if (m_effect)
	{
		m_effect->DecrementReferenceCount();
		m_effect = nullptr;
	}
}

// Implementation
//===============

eae6320::Graphics::cMesh* eae6320::cRenderComponent::GetMesh()
{
	return m_mesh;
}

eae6320::Graphics::cEffect* eae6320::cRenderComponent::GetEffect()
{
	return m_effect;
}

void eae6320::cRenderComponent::Update(const float i_deltaTime)
{
	m_rigidBody.Update(i_deltaTime);
}

void eae6320::cRenderComponent::SetVelocity(Math::sVector i_velocity)
{
	m_rigidBody.velocity = i_velocity;
}

eae6320::Math::cMatrix_transformation eae6320::cRenderComponent::GetTransform(const float i_deltaTime)
{
	return Math::cMatrix_transformation(m_rigidBody.PredictFutureOrientation(i_deltaTime), m_rigidBody.PredictFuturePosition(i_deltaTime));
}

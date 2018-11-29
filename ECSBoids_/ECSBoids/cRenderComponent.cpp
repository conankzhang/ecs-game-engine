// Includes
//=========

#include "cRenderComponent.h"

#include <Engine/Graphics/cMesh.h>
#include <Engine/Graphics/cEffect.h>
#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Platform/Platform.h>

eae6320::cRenderComponent::cRenderComponent(const char* const i_prefabPath, Math::sVector i_position, Math::cQuaternion i_orientation)
{
	LoadPrefab(i_prefabPath);

	m_rigidBody.position = i_position;
	m_rigidBody.orientation = i_orientation;
	m_maxSpeed = 20.0f;
	m_clampSpeed = false;
}

// Initialization / Clean Up
//--------------------------

eae6320::cRenderComponent::~cRenderComponent()
{
	CleanUp();
}

void eae6320::cRenderComponent::LoadPrefab(const char* const i_prefabPath)
{
	Platform::sDataFromFile dataFromFile;
	Platform::LoadBinaryFile(i_prefabPath, dataFromFile);

	auto currentOffset = reinterpret_cast<uintptr_t>( dataFromFile.data );
	const auto finalOffset = currentOffset + dataFromFile.size;

    bool isActive = *reinterpret_cast<uint8_t*>( currentOffset );

	currentOffset += sizeof( isActive );
    const auto meshPathSize = *reinterpret_cast<uint16_t*>( currentOffset );

	currentOffset += sizeof( meshPathSize );
	char* meshPath = reinterpret_cast<char*>( currentOffset );

	currentOffset += meshPathSize * sizeof( char );
	char* effectPath = reinterpret_cast<char*>( currentOffset );

	Graphics::cMesh::Load( meshPath, m_mesh );
	Graphics::cEffect::Load(m_effect, effectPath );

	SetActive(isActive);
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
	if (m_clampSpeed && m_rigidBody.velocity.GetLength() > m_maxSpeed)
	{
		m_rigidBody.velocity.Normalize();
		m_rigidBody.velocity *= m_maxSpeed;
	}
	m_rigidBody.Update(i_deltaTime);
}

void eae6320::cRenderComponent::SetVelocity(Math::sVector i_velocity)
{
	if (m_rigidBody.position.y < 5.0f && i_velocity.y < 0.0f)
	{
		m_rigidBody.velocity = Math::sVector(i_velocity.x, 0.0f, i_velocity.z);
		return;
	}

	m_rigidBody.velocity = i_velocity;
}

void eae6320::cRenderComponent::SetAcceleration(Math::sVector i_acceleration)
{
	m_rigidBody.acceleration= i_acceleration;
}

void eae6320::cRenderComponent::SetAngularSpeed(float i_speed)
{
	m_rigidBody.angularSpeed = i_speed;
}

void eae6320::cRenderComponent::SetRotationAxis(Math::sVector i_rotationAxis)
{
	m_rigidBody.angularVelocity_axis_local = i_rotationAxis;
}

eae6320::Math::sVector eae6320::cRenderComponent::GetForward()
{
	return m_rigidBody.orientation.CalculateForwardDirection();
}

eae6320::Math::sVector eae6320::cRenderComponent::GetPosition()
{
	return m_rigidBody.position;
}

void eae6320::cRenderComponent::SetPosition(Math::sVector i_position)
{
	m_rigidBody.position = i_position;
}

eae6320::Math::cMatrix_transformation eae6320::cRenderComponent::GetTransform(const float i_deltaTime)
{
	return Math::cMatrix_transformation(m_rigidBody.PredictFutureOrientation(i_deltaTime), m_rigidBody.PredictFuturePosition(i_deltaTime));
}

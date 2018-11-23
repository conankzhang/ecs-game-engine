// Includes
//=========

#include "cMyGame.h"

#include "cGameObject.h"
#include "cPrefab.h"
#include "cMeshComponent.h"
#include "cEffectComponent.h"
#include "cRigidbodyComponent.h"
#include "cPhysicsSystem.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include <Engine/Graphics/Graphics.h>
#include <Engine/Graphics/cEffect.h>
#include <Engine/Graphics/cMesh.h>
#include <Engine/Graphics/VertexFormats.h>
#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/EntityComponentSystem/IEntity.h>

// Inherited Implementation
//=========================

void eae6320::cMyGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	eae6320::Graphics::SubmitBackgroundColor(0.0f, 1.0f, 0.0f, 1.0f);
	eae6320::Graphics::SubmitCamera(m_camera->GetWorldToCameraTransform(i_elapsedSecondCount_sinceLastSimulationUpdate), m_camera->GetCameraToProjectedTransform(), i_elapsedSecondCount_systemTime, i_elapsedSecondCount_sinceLastSimulationUpdate);

	eae6320::Graphics::SubmitGameObject(m_object1->GetMesh(), m_object1->GetEffect(), m_object1->GetTransform(i_elapsedSecondCount_sinceLastSimulationUpdate));
	eae6320::Graphics::SubmitGameObject(m_object2->GetMesh(), m_object2->GetEffect(), m_object2->GetTransform(i_elapsedSecondCount_sinceLastSimulationUpdate));
	eae6320::Graphics::SubmitGameObject(m_object3->GetMesh(), m_object3->GetEffect(), m_object3->GetTransform(i_elapsedSecondCount_sinceLastSimulationUpdate));

	eae6320::ECS::IEntity* entity = ECS->GetEntityManager()->GetEntity(demoEntityId);
	eae6320::cPrefab* prefab = static_cast<eae6320::cPrefab*>(entity);
	if (prefab && prefab->IsActive() && prefab->GetComponent<cRigidbodyComponent>())
	{
		eae6320::Graphics::SubmitGameObject(entity->GetComponent<cMeshComponent>()->GetMesh(), entity->GetComponent<cEffectComponent>()->GetEffect() , entity->GetComponent<cRigidbodyComponent>()->GetTransform(i_elapsedSecondCount_sinceLastSimulationUpdate));
	}
}

// Run
//----

void eae6320::cMyGame::UpdateBasedOnInput()
{
	// Is the user pressing the ESC key?
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) )
	{
		// Exit the application
		const auto result = Exit( EXIT_SUCCESS );
		EAE6320_ASSERT( result );
	}
}

void eae6320::cMyGame::UpdateSimulationBasedOnInput()
{
	float z_camera = 0.0f;
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Up))
	{
		z_camera = -1.0f;
	}

	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Down))
	{
		z_camera = 1.0f;
	}

	float x_camera = 0.0f;
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Left))
	{
		x_camera = -1.0f;
	}

	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Right))
	{
		x_camera = 1.0f;
	}

	m_camera->SetVelocity(eae6320::Math::sVector(x_camera, 0.0f, z_camera));

	float x_object = 0.0f;
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::A))
	{
		x_object = -1.0f;
	}

	if (UserInput::IsKeyPressed(UserInput::KeyCodes::D))
	{
		x_object = 1.0f;
	}

	float y_object = 0.0f;
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::W))
	{
		y_object = 1.0f;
	}

	if (UserInput::IsKeyPressed(UserInput::KeyCodes::S))
	{
		y_object = -1.0f;
	}

	m_object1->SetVelocity(eae6320::Math::sVector(x_object, y_object, 0.0f));

	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Shift))
	{
		m_shiftPressed = true;
	}
	else
	{
		m_shiftPressed = false;
	}
}

void eae6320::cMyGame::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	m_object1->Update(i_elapsedSecondCount_sinceLastUpdate);
	m_object2->Update(i_elapsedSecondCount_sinceLastUpdate);
	m_object3->Update(i_elapsedSecondCount_sinceLastUpdate);
	m_camera->Update(i_elapsedSecondCount_sinceLastUpdate);

	ECS->Update(i_elapsedSecondCount_sinceLastUpdate);
}

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::cMyGame::Initialize()
{
	auto result = Results::Success;

	ECS = new eae6320::ECS::ECSEngine();

	eae6320::ECS::EntityManager* entityManager = ECS->GetEntityManager();
	demoEntityId = entityManager->CreateEntity<eae6320::cPrefab>("data/Prefabs/red_pyramid.prefab", eae6320::Math::sVector(-1.0f, 0.0f, 0.0f), eae6320::Math::cQuaternion());

	ECS->GetSystemManager()->AddSystem<cPhysicsSystem>();

	m_object1 = new eae6320::cGameObject(eae6320::Math::sVector(), eae6320::Math::cQuaternion());
	m_object2 = new eae6320::cGameObject(eae6320::Math::sVector(0.0f, -1.0f, 0.0f), eae6320::Math::cQuaternion());
	m_object3 = new eae6320::cGameObject(eae6320::Math::sVector(), eae6320::Math::cQuaternion());
	m_camera = new eae6320::cCamera(eae6320::Math::sVector(0.0f, 0.0f, 5.0f), eae6320::Math::cQuaternion());

	// Initialize the shading data
	{
		if ( !( result = eae6320::Graphics::cEffect::Load(s_Effect1, "data/Effects/white.effect") ) )
		{
			EAE6320_ASSERT( false );
			goto OnExit;
		}
	}

	// Initialize the geometry
	{
		if ( !( result =  eae6320::Graphics::cMesh::Load( "data/Meshes/cube.mesh", s_Mesh1 ) ) )
		{
			EAE6320_ASSERT( false );
			goto OnExit;
		}
	}

	m_object1->SetMeshAndEffect(s_Mesh1, s_Effect1);

	// Initialize the shading data
	{
		if ( !( result = eae6320::Graphics::cEffect::Load( s_Effect2, "data/Effects/red.effect") ) )
		{
			EAE6320_ASSERT( false );
			goto OnExit;
		}
	}
	// Initialize the geometry
	{
		if ( !( result =  eae6320::Graphics::cMesh::Load( "data/Meshes/plane.mesh", s_Mesh2 ) ) )
		{
			EAE6320_ASSERT( false );
			goto OnExit;
		}
	}

	m_object2->SetMeshAndEffect(s_Mesh2, s_Effect2);

	// Initialize the shading data
	{
		if ( !( result = eae6320::Graphics::cEffect::Load( s_Effect3, "data/Effects/red.effect") ) )
		{
			EAE6320_ASSERT( false );
			goto OnExit;
		}
	}
	// Initialize the geometry
	{
		if ( !( result =  eae6320::Graphics::cMesh::Load( "data/Meshes/pyramid.mesh", s_Mesh3 ) ) )
		{
			EAE6320_ASSERT( false );
			goto OnExit;
		}
	}

	m_object3->SetMeshAndEffect(s_Mesh3, s_Effect3);

OnExit:

	return result;
}

eae6320::cResult eae6320::cMyGame::CleanUp()
{
	auto result = Results::Success;

	delete ECS;

	if (m_object1)
	{
		m_object1->CleanUp();
		delete m_object1;
		m_object1 = nullptr;
	}

	if (m_object2)
	{
		m_object2->CleanUp();
		delete m_object2;
		m_object2 = nullptr;
	}

	if (m_object3)
	{
		m_object3->CleanUp();
		delete m_object3;
		m_object3 = nullptr;
	}

	if (m_camera)
	{
		delete m_camera;
		m_camera = nullptr;
	}

	return Results::Success;
}

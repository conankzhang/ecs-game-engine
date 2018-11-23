// Includes
//=========

#include "cRenderSystem.h"

#include "cCameraComponent.h"
#include "cRenderComponent.h"

#include <Engine/Graphics/Graphics.h>
#include <Engine/Math/cMatrix_transformation.h>

// Initialization / Clean Up
//--------------------------

eae6320::cRenderSystem::cRenderSystem()
{
}

eae6320::cRenderSystem::~cRenderSystem()
{

}

// Implementation
//===============

void eae6320::cRenderSystem::Update(float deltaTime)
{
	m_cameraComponent->Update(deltaTime);

	for (auto component = m_componentManager->begin<cRenderComponent>(); component != m_componentManager->end<cRenderComponent>(); ++component)
	{
		cRenderComponent* renderComponent = dynamic_cast<cRenderComponent*>(component->second);

		if (renderComponent && renderComponent->IsActive())
		{
			renderComponent->Update(deltaTime);
		}
	}
}

void eae6320::cRenderSystem::Initialize()
{
	auto cameraIterator = m_componentManager->begin<cCameraComponent>();
	m_cameraComponent = dynamic_cast<cCameraComponent*>(cameraIterator->second);
}

void eae6320::cRenderSystem::SubmitDataToBeRendered(const float i_deltaSystemTime, const float i_deltaTime)
{
	Graphics::SubmitBackgroundColor(0.13f, 0.24f, 0.33f, 1.0f);

	Graphics::SubmitCamera(m_cameraComponent->GetWorldToCameraTransform(i_deltaTime), m_cameraComponent->GetCameraToProjectedTransform(), i_deltaSystemTime, i_deltaTime);

	for (auto component = m_componentManager->begin<cRenderComponent>(); component != m_componentManager->end<cRenderComponent>(); ++component)
	{
		cRenderComponent* renderComponent = dynamic_cast<cRenderComponent*>(component->second);

		if (renderComponent && renderComponent->IsActive())
		{
			Graphics::SubmitGameObject(renderComponent->GetMesh(), renderComponent->GetEffect() , renderComponent->GetTransform(i_deltaTime));
		}
	}
}

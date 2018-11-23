// Includes
//=========

#include "cRenderSystem.h"

#include "cRenderComponent.h"

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
	for (auto component = m_componentManager->begin<cRenderComponent>(); component != m_componentManager->end<cRenderComponent>(); ++component)
	{
		cRenderComponent* renderComponent = dynamic_cast<cRenderComponent*>(component->second);

		if (renderComponent && renderComponent->IsActive())
		{
			renderComponent->Update(deltaTime);
		}
	}
}

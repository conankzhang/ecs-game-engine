/*
	Engine interface.
*/

// Includes
//=========

#include "Engine.h"

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

// Forward Declarations
//=====================

// Class Declaration
//==================

namespace eae6320
{
	namespace ECS 
	{
		ECSEngine::ECSEngine()
		{
			m_componentManager = new ComponentManager();
			m_systemManager = new SystemManager(m_componentManager);
			m_entityManager = new EntityManager(m_componentManager);
		}

		ECSEngine::~ECSEngine()
		{
			delete m_entityManager;
			m_entityManager = nullptr;

			delete m_componentManager;
			m_componentManager = nullptr;

			delete m_systemManager;
			m_systemManager = nullptr;
		}

		void ECSEngine::Update(float deltaTime)
		{
			m_systemManager->Update(deltaTime);
			m_entityManager->RemoveEntities();
		}
	}
}

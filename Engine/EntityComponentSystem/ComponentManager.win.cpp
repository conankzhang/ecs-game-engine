/*
	Component manager implementation.
*/

// Includes
//=========

#include "ComponentManager.h"
#include "Component.h"

// Forward Declarations
//=====================

// Class Declaration
//==================

namespace eae6320
{
	namespace ECS 
	{
		ComponentManager::ComponentManager()
		{ 
			m_components.resize(1024);
			m_entityComponentMap.resize(1024);
			for (std::vector<size_t> v : m_entityComponentMap)
			{
				v.resize(1024);
			}
			std::vector< std::vector<size_t> >::iterator row;
			for (row = m_entityComponentMap.begin(); row != m_entityComponentMap.end(); row++) {
				row->resize(1024);
			}
		}

		ComponentManager::~ComponentManager()
		{
			for (auto component : m_components)
			{
				delete component;
				component = nullptr;
			}
		}
	}
}

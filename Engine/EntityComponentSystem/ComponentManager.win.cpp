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
			for (auto components : m_componentMap)
			{
				for (auto component : components.second)
				{
					delete component.second;
					component.second = nullptr;
				}
			}
		}
	}
}

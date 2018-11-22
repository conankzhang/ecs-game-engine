/*
	System interface.
*/

#ifndef ISYSTEM_H
#define ISYSTEM_H 

// Includes
//=========

// Forward Declarations
//=====================
namespace eae6320
{
	namespace ECS
	{
		class ComponentManager;
	}
}
// Class Declaration
//==================

namespace eae6320
{
	namespace ECS 
	{
		class ISystem
		{
			friend class SystemManager;

		public:
			virtual ~ISystem();

			virtual void PreUpdate(float deltaTime) = 0;
			virtual void Update(float deltaTime) = 0;
			virtual void PostUpdate(float deltaTime) = 0;

			virtual inline const size_t GetSystemTypeId() const = 0;
			ComponentManager* m_componentManager;

		protected:
			ISystem(size_t i_priority = 200, float i_updateInterval = -1.0f);

		private:
			float m_deltaTime;
			float m_updateInterval;
			size_t m_systemPriority;
			bool m_isActive;
		};
	}
}

#endif	// ISYSTEM_H

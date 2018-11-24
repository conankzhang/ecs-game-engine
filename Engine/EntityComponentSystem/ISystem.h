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

			// PreUpdate loop called before Update
			virtual void PreUpdate(float deltaTime) = 0;

			// Update loop
			virtual void Update(float deltaTime) = 0;

			// PostUpdate loop called after Update
			virtual void PostUpdate(float deltaTime) = 0;

			// Similar to Start in Unity and BeginPlay in Unreal
			virtual void Initialize() {};

			// Get the unique identifier for this entity
			virtual inline const size_t GetSystemTypeId() const = 0;

		protected:
			ISystem(size_t i_priority = 200, float i_updateInterval = -1.0f);
			ComponentManager* m_componentManager;

		private:
			float m_deltaTime;
			float m_updateInterval;
			size_t m_systemPriority;
			bool m_isActive;
		};
	}
}

#endif	// ISYSTEM_H

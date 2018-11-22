/*
	Event Dispatcher interface.
*/

#ifndef IEVENTDISPATCHER_H
#define IEVENTDISPATCHER_H 

// Includes
//=========

// Forward Declarations
//=====================

// Class Declaration
//==================

namespace eae6320
{
	namespace Event 
	{
		class IEventDispatcher
		{
		public:
			virtual ~IEventDispatcher();

			virtual void Dispatch(IEvent* event) = 0;
			virtual void AddCallback(IEventDelegate* const eventDelegate) = 0;
			virtual void RemoveCallback(IEventDelegate* eventDelegate) = 0;
		};
	}
}

#endif	// IEVENTDISPATCHER_H

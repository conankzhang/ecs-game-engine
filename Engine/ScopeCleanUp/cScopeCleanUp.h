/*
	This class can be used to automatically clean up
	when a scope block ends.

	A typical usage with a lambda function would be:

		cScopeCleanUp autoCleanUp( [&]()
			{
				// Clean up code here
			} );
*/

#ifndef EAE6320_CSCOPECLEANUP_H
#define EAE6320_CSCOPECLEANUP_H

// Includes
//=========

#include <functional>

// Class Declaration
//==================

namespace eae6320
{
	class cScopeCleanUp
	{
		// Interface
		//==========

	public:

		using fCleanUpFunction = std::function<void()>;

		// Initialization / Clean Up
		//--------------------------

		cScopeCleanUp( fCleanUpFunction const i_cleanUpFunction );
		~cScopeCleanUp();

		cScopeCleanUp( const cScopeCleanUp& i_copiedFrom ) = delete;
		cScopeCleanUp( cScopeCleanUp&& i_movedFrom ) = delete;
		cScopeCleanUp& operator =( const cScopeCleanUp& i_copiedFrom ) = delete;
		cScopeCleanUp& operator =( cScopeCleanUp&& i_movedFrom ) = delete;

		// Data
		//=====

	private:

		fCleanUpFunction const m_cleanUpFunction;
	};
}

#endif	// EAE6320_CSCOPECLEANUP_H

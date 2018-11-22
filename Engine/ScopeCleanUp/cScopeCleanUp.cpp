// Includes
//=========

#include "cScopeCleanUp.h"

// Interface
//==========

// Initialization / Clean Up
//--------------------------

eae6320::cScopeCleanUp::cScopeCleanUp( fCleanUpFunction const i_cleanUpFunction )
	:
	m_cleanUpFunction( i_cleanUpFunction )
{

}

eae6320::cScopeCleanUp::~cScopeCleanUp()
{
	m_cleanUpFunction();
}

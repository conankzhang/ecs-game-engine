/*
	Type manager.
*/

#ifndef TYPEMANAGER_H
#define TYPEMANAGER_H 

// Includes
//=========

// Forward Declarations
//=====================

// Class Declaration
//==================

namespace eae6320
{
	namespace ECS 
	{
		template<class T>
		class TypeManager 
		{
		public:
		
			template<class U>
			static const size_t GetTypeId()
			{
				return s_typeCount++;
			}

			static const size_t GetTypeId()
			{
				return s_typeCount;
			}

		private:
			static size_t s_typeCount;
		};	

	}
}

#endif	// TYPEMANAGER_H

/*
	This file contains math-related functions
*/

#ifndef EAE6320_MATH_FUNCTIONS_H
#define EAE6320_MATH_FUNCTIONS_H

#include <cstdint>
#include <type_traits>

namespace eae6320
{
	namespace Math
	{
		// Interface
		//==========

		float ConvertDegreesToRadians( const float i_degrees );
		uint16_t ConvertFloatToHalf( const float i_value );
		float ConvertHorizontalFieldOfViewToVerticalFieldOfView( const float i_horizontalFieldOfView_inRadians,
			// aspectRatio = width / height
			const float i_aspectRatio );

		// Rounds up an unsigned integer to the next highest multiple
			template<typename tUnsignedInteger, class EnforceUnsigned = typename std::enable_if<std::is_unsigned<tUnsignedInteger>::value>::type>
		tUnsignedInteger RoundUpToMultiple( const tUnsignedInteger i_value, const tUnsignedInteger i_multiple );
		// If the multiple is known to be a power-of-2 this is cheaper than the previous function
			template<typename tUnsignedInteger, class EnforceUnsigned = typename std::enable_if<std::is_unsigned<tUnsignedInteger>::value>::type>
		tUnsignedInteger RoundUpToMultiple_powerOf2( const tUnsignedInteger i_value, const tUnsignedInteger i_multipleWhichIsAPowerOf2 );
	}
}

#include "Functions.hpp"

#endif	// EAE6320_MATH_FUNCTIONS_H

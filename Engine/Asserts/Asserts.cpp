// Includes
//=========

#include "Asserts.h"

#ifdef EAE6320_ASSERTS_AREENABLED
	#include <cstdarg>
	#include <cstdio>
#endif

// Helper Function Definitions
//============================

#ifdef EAE6320_ASSERTS_AREENABLED

bool eae6320::Asserts::ShowMessageIfAssertionIsFalseAndReturnWhetherToBreak( const unsigned int i_lineNumber, const char* const i_file,
	bool& io_shouldThisAssertBeIgnoredInTheFuture, const char* const i_message, ... )
{
	// Construct the message
	std::ostringstream message;
	{
		message << "An assertion failed on line " << i_lineNumber << " of " << i_file;
		constexpr size_t bufferSize = 512;
		char buffer[bufferSize];
		int formattingResult;
		{
			va_list insertions;
			va_start( insertions, i_message );
			formattingResult = vsnprintf( buffer, bufferSize, i_message, insertions );
			va_end( insertions );
		}
		if ( formattingResult != 0 )
		{
			message << ":\n\n";
			if ( formattingResult > 0 )
			{
				message << buffer;
				if ( formattingResult >= bufferSize )
				{
					message << "\n\n"
						"(The internal buffer of size " << bufferSize
						<< " was not big enough to hold the formatted message of length " << ( formattingResult + 1 ) << ")";
				}
			}
			else
			{
				message << "An encoding error occurred! The unformatted message is: \"" << i_message << "\"!";
			}
		}
		else
		{
			message << "!";
		}
	}
	// Display it and break if necessary
	return ShowMessageIfAssertionIsFalseAndReturnWhetherToBreak_platformSpecific( message, io_shouldThisAssertBeIgnoredInTheFuture );
}

#endif	// EAE6320_ASSERTS_AREENABLED

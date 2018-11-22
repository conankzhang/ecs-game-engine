// Includes
//=========

#include "cbApplication.h"

#include <algorithm>
#include <cstdlib>
#include <Engine/Asserts/Asserts.h>
#include <Engine/Graphics/Graphics.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Time/Time.h>
#include <Engine/UserOutput/UserOutput.h>

// Interface
//==========

double eae6320::Application::cbApplication::GetElapsedSecondCount_systemTime() const
{
	return Time::ConvertTicksToSeconds( m_tickCount_systemTime_current - m_tickCount_systemTime_whenApplicationStarted );
}

void eae6320::Application::cbApplication::SetSimulationRate( const float i_simulationRate )
{
	m_simulationRate = i_simulationRate;
}

// Run
//------

int eae6320::Application::cbApplication::ParseEntryPointParametersAndRun( const sEntryPointParameters& i_entryPointParameters )
{
	int exitCode = EXIT_SUCCESS;

	// Initialize the application
	// (this also starts the application loop on a separate thread)
	{
		const auto result = Initialize_all( i_entryPointParameters );
		if ( result )
		{
			Logging::OutputMessage( "The application was successfully initialized" );
		}
		else
		{
			exitCode = EXIT_FAILURE;
			EAE6320_ASSERT( false );
			Logging::OutputError( "Application initialization failed!" );
			UserOutput::Print( "Initialization failed! (Check the log file for details.) This program will now exit." );
			goto OnExit;
		}
	}
	// Enter an infinite loop rendering frames until the application is ready to exit
	{
		const auto result = RenderFramesWhileWaitingForApplicationToExit( exitCode );
		if ( !result )
		{
			EAE6320_ASSERT( false );
			UserOutput::Print( "The application encountered an error and will now exit" );
			goto OnExit;
		}
	}

OnExit:

	{
		const auto result = CleanUp_all();
		if ( !result )
		{
			EAE6320_ASSERT( false );
			if ( exitCode == EXIT_SUCCESS )
			{
				exitCode = EXIT_FAILURE;
			}
		}
	}

	return exitCode;
}

eae6320::cResult eae6320::Application::cbApplication::Exit( const int i_exitCode )
{
	const auto result = Exit_platformSpecific( i_exitCode );
	if ( result )
	{
		m_exitCode = i_exitCode;	// This will likely already be set by Exit_platformSpecific()
		m_shouldApplicationLoopExit = true;
	}
	return result;
}

// Initialization / Clean Up
//--------------------------

eae6320::Application::cbApplication::~cbApplication()
{
	CleanUp_base();
}

// Inheritable Implementation
//===========================

// Implementation
//===============

// Run
//----

void eae6320::Application::cbApplication::UpdateUntilExit()
{
	// This stores the number of ticks that have elapsed since the system has been running,
	// and it gets updated at the start of every iteration of the application loop
	auto tickCount_systemTime_currentLoop = Time::GetCurrentSystemTimeTickCount();
	m_tickCount_systemTime_current = tickCount_systemTime_currentLoop;
	// Each update of the simulation is done with a fixed amount of time
	const auto secondCount_perSimulationUpdate = GetSimulationUpdatePeriod_inSeconds();
	const uint64_t tickCount_perSimulationUpdate = Time::ConvertSecondsToTicks( secondCount_perSimulationUpdate );
	// Since a single update of the simulation only uses a fixed amount of time
	// there will be "leftover" time (time that has passed but has not been used)
	uint64_t tickCount_simulationTime_elapsedButNotYetSimulated = 0;
	// When the application is running normally all system time that passes is used to update the application.
	// There can be unusual situations, however, where a large amount of system time passes that the application should ignore.
	// One example is when you are debugging the application:
	// In that case it could be possible for a single iteration of the application loop to take minutes,
	// and the application shouldn't use all of that time or objects could move incredibly large distances in a single frame!
	// To deal with these cases there is a maximum threshold of time that a single iteration will ever use:
	uint64_t tickCount_systemTime_maxAllowablePerIteration;
	{
		constexpr auto maxAllowableTimePerIteration_inSeconds = 0.5;
		tickCount_systemTime_maxAllowablePerIteration = Time::ConvertSecondsToTicks( maxAllowableTimePerIteration_inSeconds );
	}
	// This measures how much allowable system time has elapsed
	uint64_t tickCount_systemTime_elapsedAllowable = 0;
	// It is important to keep rendering (and handling operating system messages)
	// or the application will become unresponsive,
	// and so even if the simulation is behind there is a maximum number of updates
	// that will happen in a single loop iteration before a frame is rendered
	constexpr auto maxSimulationUpdateCountWithoutRendering = 5;

	// Loop until it is time for the application to exit
	auto tickCount_simulationTime_totalElapsed = m_tickCount_simulationTime_totalElapsed;
	while ( !m_shouldApplicationLoopExit )
	{
		// Calculate how much time has elapsed since the last loop
		uint64_t tickCount_systemTime_elapsedSinceLastLoop;
		uint64_t tickCount_toSimulate_elapsedSinceLastLoop;
		{
			// Update the current system time
			// and use that to calculate how much time has elapsed since the last loop
			{
				const auto tickCount_systemTime_previousLoop = tickCount_systemTime_currentLoop;
				tickCount_systemTime_currentLoop = Time::GetCurrentSystemTimeTickCount();
				m_tickCount_systemTime_current = tickCount_systemTime_currentLoop;
				tickCount_systemTime_elapsedSinceLastLoop = tickCount_systemTime_currentLoop - tickCount_systemTime_previousLoop;
				// Only consider the allowable amount of elapsed system time
				tickCount_systemTime_elapsedSinceLastLoop =
					std::min( tickCount_systemTime_elapsedSinceLastLoop, tickCount_systemTime_maxAllowablePerIteration );
				tickCount_systemTime_elapsedAllowable += tickCount_systemTime_elapsedSinceLastLoop;
			}
			// Calculate the simulation time that has elapsed based on the simulation rate
			tickCount_toSimulate_elapsedSinceLastLoop =
				static_cast<uint64_t>( static_cast<float>( tickCount_systemTime_elapsedSinceLastLoop ) * m_simulationRate );
		}
		// Update any application state that isn't part of the simulation
		{
			UpdateBasedOnTime( static_cast<float>( Time::ConvertTicksToSeconds( tickCount_systemTime_elapsedSinceLastLoop ) ) );
			UpdateBasedOnInput();
		}
		// Update the simulation
		{
			// Add the time elapsed since the last frame to the amount of simulation time that has not been simulated yet
			tickCount_simulationTime_elapsedButNotYetSimulated += tickCount_toSimulate_elapsedSinceLastLoop;
			// Keep updating the simulation while more time has elapsed than the fixed amount used for a single update
			// (note that the expected common behavior is to render faster than the simulation is updated,
			// and so the amount of simulation updates per-iteration should most often be zero, should frequently be one,
			// and should not be more than one unless something unexpected happens that causes a single iteration to take longer than expected)
			auto simulationUpdateCount_thisIteration = 0;
			while ( ( tickCount_simulationTime_elapsedButNotYetSimulated >= tickCount_perSimulationUpdate )
				// Regardless of how far the simulation is behind
				// frames need to be rendered (and operating system messages handled)
				// or the application will stop responding
				&& ( simulationUpdateCount_thisIteration < maxSimulationUpdateCountWithoutRendering ) )
			{
				UpdateSimulationBasedOnTime( secondCount_perSimulationUpdate );
				++simulationUpdateCount_thisIteration;
				tickCount_simulationTime_totalElapsed += tickCount_perSimulationUpdate;
				m_tickCount_simulationTime_totalElapsed = tickCount_simulationTime_totalElapsed;
				tickCount_simulationTime_elapsedButNotYetSimulated -= tickCount_perSimulationUpdate;
			}
			// If a time-based simulation update happened
			// then update simulation state based on input.
			// This happens _after_ the elapsed time has been consumed,
			// because rendering has already happened with predicted extrapolation based on time that had already passed.
			// If simulation state were changed _before_ then it would invalidate frames that had already been rendered
			// (so, for example, something that had moved in a previous frame would suddenly reset to a different position).
			if ( simulationUpdateCount_thisIteration > 0 )
			{
				UpdateSimulationBasedOnInput();
			}
		}
		// Submit data for the render thread to use to render a new frame
		// after it has finished rendering the current frame with the previously-submitted data
		{
			// Wait until the render thread is ready to accept new submitted data
			{
				// Conceptually the wait is infinite
				// but practically this doesn't work because the render thread could decide that the application should exit
				// as a result of an operating system message.
				// Instead there is a loop of waits
				// so that there can be a period check of whether the application is supposed to exit.
				cResult canGraphicsDataBeSubmittedForANewFrame;
				do
				{
					// The wait is long in terms of rendering
					// but short enough that any delay in exiting will (hopefully) not be noticeable to a human
					constexpr unsigned int timeToWait_inMilliseconds = 1000 / 4;
					canGraphicsDataBeSubmittedForANewFrame = Graphics::WaitUntilDataForANewFrameCanBeSubmitted( timeToWait_inMilliseconds );
				} while ( ( canGraphicsDataBeSubmittedForANewFrame == Results::TimeOut ) && !m_shouldApplicationLoopExit );
				// If graphics data can't be submitted for a new frame the application will exit
				if ( !canGraphicsDataBeSubmittedForANewFrame )
				{
					if ( m_shouldApplicationLoopExit )
					{
						// In this case graphics data can't be submitted because the application is supposed to exit,
						// and the application is behaving normally
					}
					else
					{
						// In this case the wait failed for an unexpected reason (i.e. something other than a timeout)
						EAE6320_ASSERT( false );
						Logging::OutputError( "Failed to wait for graphics data for a new frame to be submittable" );
						UserOutput::Print( "Something unexpected went wrong and rendering can't continue (the application will now exit)" );
					}
					return;
				}
			}
			// Submit the data to be rendered
			{
				// Submit the application-specific data
				const auto elapsedSecondCount_systemTime = static_cast<float>( Time::ConvertTicksToSeconds( tickCount_systemTime_elapsedAllowable ) );
				{
					SubmitDataToBeRendered( elapsedSecondCount_systemTime,
						static_cast<float>( Time::ConvertTicksToSeconds( tickCount_simulationTime_elapsedButNotYetSimulated ) ) );
				}
				// Submit the elapsed times
				{
					const auto elapsedSecondCount_simulationTime = [tickCount_simulationTime_totalElapsed, tickCount_simulationTime_elapsedButNotYetSimulated]()
					{
						const auto tickCount_simulationTime_toRender = tickCount_simulationTime_totalElapsed + tickCount_simulationTime_elapsedButNotYetSimulated;
						return static_cast<float>( Time::ConvertTicksToSeconds( tickCount_simulationTime_toRender ) );
					}();
					Graphics::SubmitElapsedTime( elapsedSecondCount_systemTime, elapsedSecondCount_simulationTime );
				}
			}
			// Let the graphics system know that all of the data for this frame has been submitted
			// (which means that it can start using it to render)
			{
				const auto result = Graphics::SignalThatAllDataForAFrameHasBeenSubmitted();
				EAE6320_ASSERT( result );
			}
		}
	}
}

void eae6320::Application::cbApplication::EntryPoint_applicationLoopThread( void* const io_application )
{
	auto *const application = static_cast<cbApplication*>( io_application );
	EAE6320_ASSERT( application );
	return application->UpdateUntilExit();
}

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::Application::cbApplication::Initialize_all( const sEntryPointParameters& i_entryPointParameters )
{
	auto result = Results::Success;

	// Initialize logging first so that it's always available
	if ( !( result = Logging::Initialize() ) )
	{
		EAE6320_ASSERT( false );
		goto OnExit;
	}
	// Initialize time next in order to track total time
	if ( result = Time::Initialize() )
	{
		m_tickCount_systemTime_whenApplicationStarted = Time::GetCurrentSystemTimeTickCount();
	}
	else
	{
		EAE6320_ASSERT( false );
		goto OnExit;
	}
	// Initialize the new application instance with entry point parameters
	if ( !( result = Initialize_base( i_entryPointParameters ) ) )
	{
		EAE6320_ASSERT( false );
		goto OnExit;
	}
	// Initialize engine systems
	if ( !( result = Initialize_engine() ) )
	{
		EAE6320_ASSERT( false );
		goto OnExit;
	}
	// Initialize the derived application
	if ( !( result = Initialize() ) )
	{
		EAE6320_ASSERT( false );
		goto OnExit;
	}

	// Start the application loop thread
	if ( !( result = m_applicationLoopThread.Start( EntryPoint_applicationLoopThread, this ) ) )
	{
		EAE6320_ASSERT( false );
		Logging::OutputError( "The application loop thread couldn't be started" );
		goto OnExit;
	}

OnExit:

	return result;
}

eae6320::cResult eae6320::Application::cbApplication::Initialize_engine()
{
	auto result = Results::Success;

	// User Output
	{
		UserOutput::sInitializationParameters initializationParameters;
		if ( result = PopulateUserOutputInitializationParameters( initializationParameters ) )
		{
			if ( !( result = UserOutput::Initialize( initializationParameters ) ) )
			{
				EAE6320_ASSERT( false );
				goto OnExit;
			}
		}
		else
		{
			EAE6320_ASSERT( false );
			goto OnExit;
		}
	}
	// Graphics
	{
		Graphics::sInitializationParameters initializationParameters;
		if ( result = PopulateGraphicsInitializationParameters( initializationParameters ) )
		{
			if ( !( result = Graphics::Initialize( initializationParameters ) ) )
			{
				EAE6320_ASSERT( false );
				goto OnExit;
			}
		}
		else
		{
			EAE6320_ASSERT( false );
			goto OnExit;
		}
	}

OnExit:

	return result;
}

eae6320::cResult eae6320::Application::cbApplication::CleanUp_all()
{
	auto result = Results::Success;

	// Exit the application loop
	{
		// Signal to the application loop thread that it should exit
		m_shouldApplicationLoopExit = true;
		// Wait for the thread to exit
		{
			constexpr unsigned int timeToWait_inMilliseconds = 5 * 1000;
			const auto localResult = Concurrency::WaitForThreadToStop( m_applicationLoopThread, timeToWait_inMilliseconds );
			if ( !localResult )
			{
				EAE6320_ASSERTF( false, "Couldn't wait for the application loop thread to exit" );
				if ( localResult == Results::TimeOut )
				{
					Logging::OutputError( "The application loop thread didn't exit after waiting %u milliseconds", timeToWait_inMilliseconds );
				}
				if ( result )
				{
					result = localResult;
				}
			}
		}
	}
	// Clean up the derived application
	{
		const auto localResult = CleanUp();
		if ( !localResult )
		{
			EAE6320_ASSERT( false );
			if ( result )
			{
				result = localResult;
			}
		}
	}
	// Clean up engine systems
	{
		const auto localResult = CleanUp_engine();
		if ( !localResult )
		{
			EAE6320_ASSERT( false );
			if ( result )
			{
				result = localResult;
			}
		}
	}
	// Clean up the base class application
	{
		const auto localResult = CleanUp_base();
		if ( !localResult )
		{
			EAE6320_ASSERT( false );
			if ( result )
			{
				result = localResult;
			}
		}
	}
	// Clean up time second-to-last in case any clean up times are measured
	{
		const auto localResult = Time::CleanUp();
		if ( !localResult )
		{
			EAE6320_ASSERT( false );
			if ( result )
			{
				result = localResult;
			}
		}
	}
	// Clean up logging last so that messages can still be logged during clean up
	{
		const auto localResult = Logging::CleanUp();
		if ( !localResult )
		{
			EAE6320_ASSERT( false );
			if ( result )
			{
				result = localResult;
			}
		}
	}

	return result;
}

eae6320::cResult eae6320::Application::cbApplication::CleanUp_engine()
{
	auto result = Results::Success;

	// Graphics
	{
		const auto localResult = Graphics::CleanUp();
		if ( !localResult )
		{
			EAE6320_ASSERT( false );
			if ( result )
			{
				result = localResult;
			}
		}
	}
	// User Output
	{
		const auto localResult = UserOutput::CleanUp();
		if ( !localResult )
		{
			EAE6320_ASSERT( false );
			if ( result )
			{
				result = localResult;
			}
		}
	}

	return result;
}

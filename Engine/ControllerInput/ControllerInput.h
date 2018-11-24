#pragma once

#include <cstdint>
#include <functional>
#include "ExternalLibraries.h"
#include "Engine/Math/sVector.h"
#include "Engine/Results/Results.h"

namespace eae6320
{
	namespace UserInput
	{
		namespace ControllerInput
		{
			enum ControllerKeyCodes
			{
				DPAD_UP = 0x0001,
				DPAD_DOWN = 0x0002,
				DPAD_LEFT = 0x0004,
				DPAD_RIGHT = 0x0008,
				START = 0x0010,
				BACK = 0x0020,
				LEFT_THUMB = 0x0040,
				RIGHT_THUMB = 0x0080,
				LEFT_SHOULDER = 0x0100,
				RIGHT_SHOULDER = 0x0200,
				A = 0x1000,
				B = 0x2000,
				X = 0x4000,
				Y = 0x8000,
				LEFT_TRIGGER,
				RIGHT_TRIGGER,
				LEFT_STICK,
				RIGHT_STICK
			};

			//Initialize
			eae6320::cResult Initialize();

			//Update
			eae6320::cResult Update(LPVOID i_InParameter);

			//Cleanup
			eae6320::cResult CleanUp();


			//All functions have a default parameter for controller number set to 0
			//Which can be set to values between 0 and 3

			//Check if a key or trigger is pressed.
			bool IsKeyPressed(ControllerKeyCodes i_KeyCode, uint8_t i_ControllerNumber = 0);

			//Get the actual magnitude of trigger deflection.
			//Returns only the magnitude and not the x and y axis values if used for sticks.
			float GetTriggerDeflection(ControllerKeyCodes i_KeyCode, uint8_t i_ControllerNumber = 0);

			//Get the normalized magnitude between 0.0 and 1.0 of trigger deflection.
			//Returns only the normalized magnitude and not the x and y axis values if used for sticks.
			float GetNormalizedTriggerDeflection(ControllerKeyCodes i_KeyCode, uint8_t i_ControllerNumber = 0);

			//Get the magnitude of deflection in the X and Y axis for the sticks.
			eae6320::Math::sVector GetStickDeflection(ControllerKeyCodes i_KeyCode, uint8_t i_ControllerNumber = 0);

			//Get the normalized magnitude between 0.0 and 1.0 of deflection in the X and Y axis for the sticks.
			eae6320::Math::sVector GetNormalizedStickDeflection(ControllerKeyCodes i_KeyCode, uint8_t i_ControllerNumber = 0);

			//Set the individual rotation speed for each of the motors in the controller. 
			void SetVibrationEffects(uint16_t i_LowFrequencySpeed, uint16_t i_HighFrequencySpeed, uint8_t i_ControllerNumber = 0);

			//Set the same speed to both the motors in the controller.
			void SetVibrationEffects(uint16_t i_Speed, uint8_t i_ControllerNumber = 0);

			//Register Functions for callback
			void RegisterFunctionForCallback(ControllerKeyCodes i_KeyCode, std::function<void()> i_CallbackFunction, uint8_t i_ControllerNumber = 0);

			//Unregister function from callback
			eae6320::cResult RemoveKeyFromCallback(ControllerKeyCodes i_KeyCode, uint8_t i_ControllerNumber = 0);

			//Unregister all functions associated with a key.
			//eae6320::cResult RemoveFunctionFromCallback(std::function<void()> i_CallbackFunction, uint8_t i_ControllerNumber = 0);

			//Other Functions
			uint8_t GetNumberOfConnectedControllers();
		}
	}
}

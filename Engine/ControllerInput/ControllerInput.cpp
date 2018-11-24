#include "ControllerInput.h"
#include <cmath>
#include <map>
#include <future>
#include <Engine/Asserts/Asserts.h>
#include "Engine/Time/Time.h"
#include "Engine/Logging/Logging.h"
#include <Engine/Platform/Platform.h>
#include <Engine/ScopeCleanUp/cScopeCleanUp.h>
#include <External/Lua/Includes.h>
#include <Engine/Concurrency/cThread.h>
namespace
{
	uint8_t g_NumberOfConnectedControllers = 0;
	bool g_bIsCalledFirstTime = true;
	float g_elapsedTime;
	struct ControllerState
	{
		XINPUT_STATE state;
	};
	uint64_t g_PreviousTickCount;
	ControllerState g_Controllers[XUSER_MAX_COUNT];

	//For Function callback
	std::map<uint8_t, std::vector<std::map < eae6320::UserInput::ControllerInput::ControllerKeyCodes, std::function<void()>>>> g_FunctionLookupTable;

	std::map<uint8_t, std::map<eae6320::UserInput::ControllerInput::ControllerKeyCodes, uint16_t>> g_KeyMapping;

	std::map<uint8_t, std::vector<uint16_t>> g_DeadZones;

	//Threading Variables
	bool g_IsThreadRunning = false;
	eae6320::Concurrency::cThread g_UpdateThread;


	//To load data from LUA
	constexpr auto* const s_userSettingsFileName = "settings.ini";
	jmp_buf s_jumpBuffer;

	//To load data from binary file.
	constexpr auto* const s_controllerSettingsFileName = "data/controllerSettings.dat";
}

namespace
{
	eae6320::cResult InitializeIfNecessary();
	eae6320::cResult LoadUserSettingsIntoLuaTable(lua_State& io_luaState);
	eae6320::cResult PopulateUserSettingsFromLuaTable(lua_State& io_luaState);
	eae6320::cResult PopulateMapValuesForIndividualControllers(lua_State& io_luaState, uint8_t i_ControllerNumber);

	// Called if Lua panics
	// (e.g. when an unhandled error is thrown)
	int OnLuaPanic(lua_State* io_luaState) noexcept;
}

namespace
{
	int g_DefinedControllersInSettings = 0;
	uint16_t GetValueForKeyCode(std::string);
	void InitializeDefaultValues();
	eae6320::cResult LoadControllerSettings();
	//template<typename T, typename... U>
	//size_t GetAddress(std::function<T(U...)> f);
}

namespace
{
	uint16_t g_LeftStickDeadZone = 0;
	uint16_t g_RightStickDeadZone = 0;
	uint16_t g_TriggerDeadZone = 0;
	eae6320::cResult LoadValuesFromBinary();
}

//Check For new controllers
void CheckForNewControllers()
{
	DWORD dwResult;
	uint8_t temp = 0;
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		dwResult = XInputGetState(i, &state);
		if (dwResult == ERROR_SUCCESS)
		{
			temp++;
		}
	}
	g_NumberOfConnectedControllers = temp;
}

void CallCallbackFunction(eae6320::UserInput::ControllerInput::ControllerKeyCodes i_KeyCode, uint8_t i_ControllerNumber = 0)
{
	auto temp = g_FunctionLookupTable[i_ControllerNumber];

	for (auto&x:temp)
	{
		if (x[i_KeyCode])
		{
			std::async(x[i_KeyCode]);
		}
	}
}

bool eae6320::UserInput::ControllerInput::IsKeyPressed(ControllerKeyCodes i_KeyCode, uint8_t i_ControllerNumber)
{
	if ((i_ControllerNumber >= 0 && i_ControllerNumber < 4) && g_NumberOfConnectedControllers > 0)
	{
		XINPUT_STATE state = g_Controllers[i_ControllerNumber].state;
		if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_TRIGGER) || (i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_TRIGGER))
		{
			if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_TRIGGER))
			{
				if (state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
				{
					CallCallbackFunction(i_KeyCode, i_ControllerNumber);
					return true;
				}
			}
			else if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_TRIGGER))
			{
				if (state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
				{
					CallCallbackFunction(i_KeyCode, i_ControllerNumber);
					return true;
				}
			}
		}
		else
		{
			if ((state.Gamepad.wButtons & g_KeyMapping[i_ControllerNumber][i_KeyCode]) != 0)
			{
				CallCallbackFunction(i_KeyCode, i_ControllerNumber);
				return true;
			}
		}
	}
	return false;
}

float eae6320::UserInput::ControllerInput::GetTriggerDeflection(ControllerKeyCodes i_KeyCode, uint8_t i_ControllerNumber)
{
	if ((i_ControllerNumber >= 0 && i_ControllerNumber < 4) && g_NumberOfConnectedControllers > 0)
	{
		XINPUT_STATE state = g_Controllers[i_ControllerNumber].state;
		if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_TRIGGER) || (i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_TRIGGER))
		{
			auto threshold = g_DeadZones[i_ControllerNumber][2] > 0 ? g_DeadZones[i_ControllerNumber][2] : XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
			//Check for deadzones
			if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_TRIGGER))
			{
				float def = state.Gamepad.bLeftTrigger;

				return (def > threshold) ? def - threshold : 0;
			}
			else if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_TRIGGER))
			{
				float def = state.Gamepad.bRightTrigger;

				return (def > threshold) ? def - threshold : 0;
			}
		}
	}
	return 0;
}

float eae6320::UserInput::ControllerInput::GetNormalizedTriggerDeflection(ControllerKeyCodes i_KeyCode, uint8_t i_ControllerNumber)
{
	if ((i_ControllerNumber >= 0 && i_ControllerNumber < 4) && g_NumberOfConnectedControllers > 0)
	{
		XINPUT_STATE state = g_Controllers[i_ControllerNumber].state;
		auto threshold = g_DeadZones[i_ControllerNumber][2] > 0 ? g_DeadZones[i_ControllerNumber][2] : XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
		if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_TRIGGER) || (i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_TRIGGER))
		{
			//Check for deadzones
			if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_TRIGGER))
			{
				float def = state.Gamepad.bLeftTrigger;

				return (def > threshold) ? (def - threshold) / (255 - threshold) : 0;
			}
			else if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_TRIGGER))
			{
				float def = state.Gamepad.bRightTrigger;

				return (def > threshold) ? (def - threshold) / (255 - threshold) : 0;
			}
		}
	}
	return 0;
}

eae6320::Math::sVector eae6320::UserInput::ControllerInput::GetStickDeflection(ControllerKeyCodes i_KeyCode, uint8_t i_ControllerNumber)
{
	if ((i_ControllerNumber >= 0 && i_ControllerNumber < 4) && g_NumberOfConnectedControllers > 0)
	{
		XINPUT_STATE state = g_Controllers[i_ControllerNumber].state;
		if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_STICK) || (i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_STICK))
		{
			//Check for deadzones
			if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_STICK))
			{
				auto threshold = g_DeadZones[i_ControllerNumber][0] > 0 ? g_DeadZones[i_ControllerNumber][0] : XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
				float LX = state.Gamepad.sThumbLX;
				float LY = state.Gamepad.sThumbLY;

				//determine how far the controller is pushed
				float magnitude = sqrt(LX*LX + LY * LY);

				//check if the controller is outside a circular dead zone
				if (magnitude > threshold)
				{
					return eae6320::Math::sVector(LX, LY, 0);
				}
				else //if the controller is in the deadzone zero out the magnitude
				{
					return eae6320::Math::sVector(0, 0, 0);
				}
			}
			else if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_STICK))
			{
				auto threshold = g_DeadZones[i_ControllerNumber][1] > 0 ? g_DeadZones[i_ControllerNumber][1] : XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
				float RX = state.Gamepad.sThumbRX;
				float RY = state.Gamepad.sThumbRY;

				//determine how far the controller is pushed
				float magnitude = sqrt(RX*RX + RY * RY);

				//check if the controller is outside a circular dead zone
				if (magnitude > threshold)
				{
					return eae6320::Math::sVector(RX, RY, 0);
				}
				else //if the controller is in the deadzone zero out the magnitude
				{
					return eae6320::Math::sVector(0, 0, 0);
				}
				
			}
		}
	}

	return eae6320::Math::sVector();
}

eae6320::Math::sVector eae6320::UserInput::ControllerInput::GetNormalizedStickDeflection(ControllerKeyCodes i_KeyCode, uint8_t i_ControllerNumber)
{
	if ((i_ControllerNumber >= 0 && i_ControllerNumber < 4) && g_NumberOfConnectedControllers > 0)
	{
		XINPUT_STATE state = g_Controllers[i_ControllerNumber].state;
		if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_STICK) || (i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_STICK))
		{
			//Check for deadzones
			if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_STICK))
			{
				auto threshold = g_DeadZones[i_ControllerNumber][0] > 0 ? g_DeadZones[i_ControllerNumber][0] : XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
				float LX = state.Gamepad.sThumbLX;
				float LY = state.Gamepad.sThumbLY;

				//determine how far the controller is pushed
				float magnitude = sqrt(LX*LX + LY * LY);

				//determine the direction the controller is pushed
				float normalizedLX = LX / magnitude;
				float normalizedLY = LY / magnitude;

				if (magnitude > threshold)
				{
					return eae6320::Math::sVector(normalizedLX, normalizedLY, 0);
				}
				else
				{
					return eae6320::Math::sVector(0, 0, 0);
				}
				
			}
			else if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_STICK))
			{
				auto threshold = g_DeadZones[i_ControllerNumber][0] > 0 ? g_DeadZones[i_ControllerNumber][0] : XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
				float RX = state.Gamepad.sThumbRX;
				float RY = state.Gamepad.sThumbRY;

				//determine how far the controller is pushed
				float magnitude = sqrt(RX*RX + RY * RY);

				//determine the direction the controller is pushed
				float normalizedRX = RX / magnitude;
				float normalizedRY = RY / magnitude;

				if (magnitude > threshold)
				{
					return eae6320::Math::sVector(normalizedRX, normalizedRY, 0);
				}
				else
				{
					return eae6320::Math::sVector(0, 0, 0);
				}
			}
		}
	}

	return eae6320::Math::sVector();
}

void eae6320::UserInput::ControllerInput::SetVibrationEffects(uint16_t i_LowFrequencySpeed, uint16_t i_HighFrequencySpeed, uint8_t i_ControllerNumber)
{
	if ((i_ControllerNumber >= 0 && i_ControllerNumber < 4) && g_NumberOfConnectedControllers > 0)
	{
		XINPUT_VIBRATION vib;
		ZeroMemory(&vib, sizeof(XINPUT_VIBRATION));
		vib.wLeftMotorSpeed = i_LowFrequencySpeed;
		vib.wRightMotorSpeed = i_HighFrequencySpeed;
		XInputSetState(i_ControllerNumber, &vib);
	}
}

void eae6320::UserInput::ControllerInput::SetVibrationEffects(uint16_t i_Speed, uint8_t i_ControllerNumber)
{
	if ((i_ControllerNumber >= 0 && i_ControllerNumber < 4) && g_NumberOfConnectedControllers > 0)
	{
		XINPUT_VIBRATION vib;
		ZeroMemory(&vib, sizeof(XINPUT_VIBRATION));
		vib.wLeftMotorSpeed = i_Speed;
		vib.wRightMotorSpeed = i_Speed;
		XInputSetState(i_ControllerNumber, &vib);
	}
}

void eae6320::UserInput::ControllerInput::RegisterFunctionForCallback(ControllerKeyCodes i_KeyCode, std::function<void()> i_CallbackFunction, uint8_t i_ControllerNumber)
{
	std::map < eae6320::UserInput::ControllerInput::ControllerKeyCodes, std::function<void()>> temp;
	temp[i_KeyCode] = i_CallbackFunction;
	g_FunctionLookupTable[i_ControllerNumber].push_back(temp);
}

eae6320::cResult eae6320::UserInput::ControllerInput::RemoveKeyFromCallback(ControllerKeyCodes i_KeyCode, uint8_t i_ControllerNumber)
{
	auto result = eae6320::Results::Success;
	for (auto& x: g_FunctionLookupTable[i_ControllerNumber])
	{
		auto it = x.find(i_KeyCode);
		if (it != x.end())
		{
			x.erase(it);
		}
	}
	return result;
}

//eae6320::cResult eae6320::UserInput::ControllerInput::RemoveFunctionFromCallback(std::function<void()> i_CallbackFunction, uint8_t i_ControllerNumber)
//{
//	auto result = eae6320::Results::Success;
//	for (auto& temp : g_FunctionLookupTable)
//	{
//		for (auto& x : temp.second)
//		{
//			for (auto& y : x)
//			{
//				if (GetAddress(y.second) == GetAddress(i_CallbackFunction))
//				{
//					x.erase(y.first);
//				}
//			}
//		}
//	}
//	return result;
//}

uint8_t eae6320::UserInput::ControllerInput::GetNumberOfConnectedControllers()
{
	return g_NumberOfConnectedControllers;
}

eae6320::cResult eae6320::UserInput::ControllerInput::Initialize()
{
	eae6320::cResult result = Results::Success;
	CheckForNewControllers();
	LoadControllerSettings();
	if (!g_IsThreadRunning)
	{
		g_IsThreadRunning = true;
		result = g_UpdateThread.Start(Update);
		if (result)
		{
			g_IsThreadRunning = true;
		}
	}
	else
	{
		result = Results::Failure;
	}
	return result;
}

eae6320::cResult eae6320::UserInput::ControllerInput::Update(LPVOID i_InParameter)
{
	DWORD dwResult = 0;
	eae6320::cResult result = eae6320::Results::Success;
	while (g_IsThreadRunning)
	{
		if (g_bIsCalledFirstTime || (g_elapsedTime >= 5))
		{
			if (g_bIsCalledFirstTime) { g_bIsCalledFirstTime = false; }
			CheckForNewControllers();
			g_PreviousTickCount = eae6320::Time::GetCurrentSystemTimeTickCount();
			g_elapsedTime = 0.f;
		}
		if (g_NumberOfConnectedControllers == 0) { dwResult = 1; }
		g_elapsedTime += static_cast<float>(eae6320::Time::ConvertTicksToSeconds(eae6320::Time::GetCurrentSystemTimeTickCount() - g_PreviousTickCount));
		g_PreviousTickCount = eae6320::Time::GetCurrentSystemTimeTickCount();
		for (DWORD i = 0; i < g_NumberOfConnectedControllers; i++)
		{
			XINPUT_STATE state;
			ZeroMemory(&state, sizeof(XINPUT_STATE));

			dwResult = XInputGetState(i, &state);
			if (dwResult == ERROR_SUCCESS)
			{
				if (!(g_Controllers[i].state.dwPacketNumber == state.dwPacketNumber))
				{
					g_Controllers[i].state = state;
				}
			}
		}

		for (uint8_t i = 0; i < g_NumberOfConnectedControllers; i++)
		{
			auto temp = g_FunctionLookupTable[i];
			for (auto& x : temp)
			{
				for (auto& y:x )
				{
					IsKeyPressed(y.first, i);
				}
			}
		}
	}
	return result;
}

eae6320::cResult eae6320::UserInput::ControllerInput::CleanUp()
{
	g_IsThreadRunning = false;
	eae6320::Concurrency::WaitForThreadToStop(g_UpdateThread);
	return Results::Success;
}


namespace
{
	uint16_t GetValueForKeyCode(std::string s)
	{
		if (s == "A")
		{
			return 0x1000;
		}
		else if (s == "B")
		{
			return 0x2000;
		}
		else if (s == "X")
		{
			return 0x4000;
		}
		else if (s == "Y")
		{
			return 0x8000;
		}
		else if (s == "DPAD_UP")
		{
			return 0x0001;
		}
		else if (s == "DPAD_DOWN")
		{
			return 0x0002;
		}
		else if (s == "DPAD_LEFT")
		{
			return 0x0004;
		}
		else if (s == "DPAD_RIGHT")
		{
			return 0x0008;
		}
		else if (s == "START")
		{
			return 0x0010;
		}
		else if (s == "BACK")
		{
			return 0x0020;
		}
		else if (s == "LEFT_THUMB")
		{
			return 0x0040;
		}
		else if (s == "RIGHT_THUMB")
		{
			return 0x0080;
		}
		else if (s == "LEFT_SHOULDER")
		{
			return 0x0100;
		}
		else if (s == "RIGHT_SHOULDER")
		{
			return 0x0200;
		}
		return 0x0;
	}

	void InitializeDefaultValues()
	{
		for (uint8_t i = g_DefinedControllersInSettings; i < g_NumberOfConnectedControllers; i++)
		{
			g_KeyMapping[i][eae6320::UserInput::ControllerInput::DPAD_UP] = 0x0001;
			g_KeyMapping[i][eae6320::UserInput::ControllerInput::DPAD_DOWN] = 0x0002;
			g_KeyMapping[i][eae6320::UserInput::ControllerInput::DPAD_LEFT] = 0x0004;
			g_KeyMapping[i][eae6320::UserInput::ControllerInput::DPAD_RIGHT] = 0x0008;
			g_KeyMapping[i][eae6320::UserInput::ControllerInput::START] = 0x0010;
			g_KeyMapping[i][eae6320::UserInput::ControllerInput::BACK] = 0x0020;
			g_KeyMapping[i][eae6320::UserInput::ControllerInput::LEFT_THUMB] = 0x0040;
			g_KeyMapping[i][eae6320::UserInput::ControllerInput::RIGHT_THUMB] = 0x0080;
			g_KeyMapping[i][eae6320::UserInput::ControllerInput::LEFT_SHOULDER] = 0x0100;
			g_KeyMapping[i][eae6320::UserInput::ControllerInput::RIGHT_SHOULDER] = 0x0200;
			g_KeyMapping[i][eae6320::UserInput::ControllerInput::A] = 0x1000;
			g_KeyMapping[i][eae6320::UserInput::ControllerInput::B] = 0x2000;
			g_KeyMapping[i][eae6320::UserInput::ControllerInput::X] = 0x4000;
			g_KeyMapping[i][eae6320::UserInput::ControllerInput::Y] = 0x8000;
			g_DeadZones[i].push_back(0);
			g_DeadZones[i].push_back(0);
			g_DeadZones[i].push_back(0);
		}
	}
}

namespace
{

	eae6320::cResult PopulateUserSettingsFromLuaTable(lua_State& io_luaState)
	{
		auto result = eae6320::Results::Success;

		// There should always be enough stack space because the file had to be loaded,
		// but it doesn't hurt to do a sanity check in the context of this function
		if (!lua_checkstack(&io_luaState, 1))
		{
			EAE6320_ASSERTF(false, "Not enough stack space to read a setting from user settings file");
			eae6320::Logging::OutputError("Lua can't allocate enough stack space to read each user setting");
			return eae6320::Results::OutOfMemory;
		}

		{
			const char* key = "controller1";
			lua_pushstring(&io_luaState, key);
			lua_gettable(&io_luaState, -2);
			if (lua_istable(&io_luaState, -1))
			{
				PopulateMapValuesForIndividualControllers(io_luaState, 0);
				g_DefinedControllersInSettings++;
			}
			lua_pop(&io_luaState, 1);
		}
		{
			const char* key = "controller2";
			lua_pushstring(&io_luaState, key);
			lua_gettable(&io_luaState, -2);
			if (lua_istable(&io_luaState, -1))
			{
				PopulateMapValuesForIndividualControllers(io_luaState, 1);
				g_DefinedControllersInSettings++;
			}
			lua_pop(&io_luaState, 1);
		}
		{
			const char* key = "controller3";
			lua_pushstring(&io_luaState, key);
			lua_gettable(&io_luaState, -2);
			if (lua_istable(&io_luaState, -1))
			{
				PopulateMapValuesForIndividualControllers(io_luaState, 2);
				g_DefinedControllersInSettings++;
			}
			lua_pop(&io_luaState, 1);
		}
		{
			const char* key = "controller4";
			lua_pushstring(&io_luaState, key);
			lua_gettable(&io_luaState, -2);
			if (lua_istable(&io_luaState, -1))
			{
				PopulateMapValuesForIndividualControllers(io_luaState, 3);
				g_DefinedControllersInSettings++;
			}
			lua_pop(&io_luaState, 1);
		}
		InitializeDefaultValues();
		return result;
	}

	eae6320::cResult PopulateMapValuesForIndividualControllers(lua_State& io_luaState, uint8_t i_ControllerNumber)
	{
		// A
		{
			const char* key_width = "A";

			lua_pushstring(&io_luaState, key_width);
			lua_gettable(&io_luaState, -2);
			if (lua_isstring(&io_luaState, -1))
			{
				auto newCode = lua_tostring(&io_luaState, -1);
				uint16_t temp = GetValueForKeyCode(newCode);
				g_KeyMapping[i_ControllerNumber][eae6320::UserInput::ControllerInput::A] = temp;
			}
			lua_pop(&io_luaState, 1);
		}
		// B
		{
			const char* key_width = "B";

			lua_pushstring(&io_luaState, key_width);
			lua_gettable(&io_luaState, -2);
			if (lua_isstring(&io_luaState, -1))
			{
				auto newCode = lua_tostring(&io_luaState, -1);
				uint16_t temp = GetValueForKeyCode(newCode);
				g_KeyMapping[i_ControllerNumber][eae6320::UserInput::ControllerInput::B] = temp;
			}
			lua_pop(&io_luaState, 1);
		}
		// X
		{
			const char* key_width = "X";

			lua_pushstring(&io_luaState, key_width);
			lua_gettable(&io_luaState, -2);
			if (lua_isstring(&io_luaState, -1))
			{
				auto newCode = lua_tostring(&io_luaState, -1);
				uint16_t temp = GetValueForKeyCode(newCode);
				g_KeyMapping[i_ControllerNumber][eae6320::UserInput::ControllerInput::X] = temp;
			}
			lua_pop(&io_luaState, 1);
		}
		// Y
		{
			const char* key_width = "Y";

			lua_pushstring(&io_luaState, key_width);
			lua_gettable(&io_luaState, -2);
			if (lua_isstring(&io_luaState, -1))
			{
				auto newCode = lua_tostring(&io_luaState, -1);
				uint16_t temp = GetValueForKeyCode(newCode);
				g_KeyMapping[i_ControllerNumber][eae6320::UserInput::ControllerInput::Y] = temp;
			}
			lua_pop(&io_luaState, 1);
		}
		// DPAD_UP
		{
			const char* key_width = "DPAD_UP";

			lua_pushstring(&io_luaState, key_width);
			lua_gettable(&io_luaState, -2);
			if (lua_isstring(&io_luaState, -1))
			{
				auto newCode = lua_tostring(&io_luaState, -1);
				uint16_t temp = GetValueForKeyCode(newCode);
				g_KeyMapping[i_ControllerNumber][eae6320::UserInput::ControllerInput::DPAD_UP] = temp;
			}
			lua_pop(&io_luaState, 1);
		}
		// DPAD_DOWN
		{
			const char* key_width = "DPAD_DOWN";

			lua_pushstring(&io_luaState, key_width);
			lua_gettable(&io_luaState, -2);
			if (lua_isstring(&io_luaState, -1))
			{
				auto newCode = lua_tostring(&io_luaState, -1);
				uint16_t temp = GetValueForKeyCode(newCode);
				g_KeyMapping[i_ControllerNumber][eae6320::UserInput::ControllerInput::DPAD_DOWN] = temp;
			}
			lua_pop(&io_luaState, 1);
		}
		// DPAD_LEFT
		{
			const char* key_width = "DPAD_LEFT";

			lua_pushstring(&io_luaState, key_width);
			lua_gettable(&io_luaState, -2);
			if (lua_isstring(&io_luaState, -1))
			{
				auto newCode = lua_tostring(&io_luaState, -1);
				uint16_t temp = GetValueForKeyCode(newCode);
				g_KeyMapping[i_ControllerNumber][eae6320::UserInput::ControllerInput::DPAD_LEFT] = temp;
			}
			lua_pop(&io_luaState, 1);
		}
		// DPAD_RIGHT
		{
			const char* key_width = "DPAD_RIGHT";

			lua_pushstring(&io_luaState, key_width);
			lua_gettable(&io_luaState, -2);
			if (lua_isstring(&io_luaState, -1))
			{
				auto newCode = lua_tostring(&io_luaState, -1);
				uint16_t temp = GetValueForKeyCode(newCode);
				g_KeyMapping[i_ControllerNumber][eae6320::UserInput::ControllerInput::DPAD_RIGHT] = temp;
			}
			lua_pop(&io_luaState, 1);
		}
		// START
		{
			const char* key_width = "START";

			lua_pushstring(&io_luaState, key_width);
			lua_gettable(&io_luaState, -2);
			if (lua_isstring(&io_luaState, -1))
			{
				auto newCode = lua_tostring(&io_luaState, -1);
				uint16_t temp = GetValueForKeyCode(newCode);
				g_KeyMapping[i_ControllerNumber][eae6320::UserInput::ControllerInput::START] = temp;
			}
			lua_pop(&io_luaState, 1);
		}
		// BACK
		{
			const char* key_width = "BACK";

			lua_pushstring(&io_luaState, key_width);
			lua_gettable(&io_luaState, -2);
			if (lua_isstring(&io_luaState, -1))
			{
				auto newCode = lua_tostring(&io_luaState, -1);
				uint16_t temp = GetValueForKeyCode(newCode);
				g_KeyMapping[i_ControllerNumber][eae6320::UserInput::ControllerInput::BACK] = temp;
			}
			lua_pop(&io_luaState, 1);
		}
		// Left Thumb
		{
			const char* key_width = "LEFT_THUMB";

			lua_pushstring(&io_luaState, key_width);
			lua_gettable(&io_luaState, -2);
			if (lua_isstring(&io_luaState, -1))
			{
				auto newCode = lua_tostring(&io_luaState, -1);
				uint16_t temp = GetValueForKeyCode(newCode);
				g_KeyMapping[i_ControllerNumber][eae6320::UserInput::ControllerInput::LEFT_THUMB] = temp;
			}
			lua_pop(&io_luaState, 1);
		}
		// Right Thumb
		{
			const char* key_width = "RIGHT_THUMB";

			lua_pushstring(&io_luaState, key_width);
			lua_gettable(&io_luaState, -2);
			if (lua_isstring(&io_luaState, -1))
			{
				auto newCode = lua_tostring(&io_luaState, -1);
				uint16_t temp = GetValueForKeyCode(newCode);
				g_KeyMapping[i_ControllerNumber][eae6320::UserInput::ControllerInput::RIGHT_THUMB] = temp;
			}
			lua_pop(&io_luaState, 1);
		}
		// LB
		{
			const char* key_width = "LEFT_SHOULDER";

			lua_pushstring(&io_luaState, key_width);
			lua_gettable(&io_luaState, -2);
			if (lua_isstring(&io_luaState, -1))
			{
				auto newCode = lua_tostring(&io_luaState, -1);
				uint16_t temp = GetValueForKeyCode(newCode);
				g_KeyMapping[i_ControllerNumber][eae6320::UserInput::ControllerInput::LEFT_SHOULDER] = temp;
			}
			lua_pop(&io_luaState, 1);
		}
		// RB
		{
			const char* key_width = "RIGHT_SHOULDER";

			lua_pushstring(&io_luaState, key_width);
			lua_gettable(&io_luaState, -2);
			if (lua_isstring(&io_luaState, -1))
			{
				auto newCode = lua_tostring(&io_luaState, -1);
				uint16_t temp = GetValueForKeyCode(newCode);
				g_KeyMapping[i_ControllerNumber][eae6320::UserInput::ControllerInput::RIGHT_SHOULDER] = temp;
			}
			lua_pop(&io_luaState, 1);
		}
		//Left Stick Dead Zone
		{
			const char* key_width = "LeftStickDeadZone";

			lua_pushstring(&io_luaState, key_width);
			lua_gettable(&io_luaState, -2);
			if (lua_isstring(&io_luaState, -1))
			{
				if (!(lua_tonumber(&io_luaState, -1) > 65535))
				{
					g_DeadZones[i_ControllerNumber].push_back(static_cast<uint16_t>(lua_tonumber(&io_luaState, -1)));
				}
			}
			lua_pop(&io_luaState, 1);
		}
		//Right Stick Dead Zone
		{
			const char* key_width = "RightStickDeadZone";

			lua_pushstring(&io_luaState, key_width);
			lua_gettable(&io_luaState, -2);
			if (lua_isstring(&io_luaState, -1))
			{
				if (!(lua_tonumber(&io_luaState, -1) > 65535))
				{
					g_DeadZones[i_ControllerNumber].push_back(static_cast<uint16_t>(lua_tonumber(&io_luaState, -1)));
				}
			}
			lua_pop(&io_luaState, 1);
		}
		//Trigger Dead Zone
		{
			const char* key_width = "TriggerDeadZone";

			lua_pushstring(&io_luaState, key_width);
			lua_gettable(&io_luaState, -2);
			if (lua_isstring(&io_luaState, -1))
			{
				if (!(lua_tonumber(&io_luaState, -1) > 255))
				{
					g_DeadZones[i_ControllerNumber].push_back(static_cast<uint16_t>(lua_tonumber(&io_luaState, -1)));
				}
			}
			lua_pop(&io_luaState, 1);
		}
		return eae6320::Results::Success;
	}

	int OnLuaPanic(lua_State* io_luaState) noexcept
	{
		// The error object should be at the top of the stack
		{
			if (lua_isstring(io_luaState, -1))
			{
				eae6320::Logging::OutputError("Lua is panicking when processing User Settings: %s", lua_tostring(io_luaState, -1));
			}
			else
			{
				eae6320::Logging::OutputError("Lua is panicking for an unknown reason when processing User Settings");
			}
		}
		// Jump back to the restore point
		{
			constexpr int errorCode = -1;	// This value isn't checked
			longjmp(s_jumpBuffer, errorCode);
		}

		// This code should never be reached
		eae6320::Logging::OutputError("The application was not able to recover from Lua's panicking."
			" It is about to be forcefully exited.");
		return 0;
	}

	eae6320::cResult LoadUserSettingsIntoLuaTable(lua_State& io_luaState)
	{
		// Load the user settings file into the Lua environment
		if (eae6320::Platform::DoesFileExist(s_userSettingsFileName))
		{
			constexpr int requiredStackSlotCount = 0
				// The file as a function
				+ 1
				// The environment upvalue
				+ 1
				;
			if (lua_checkstack(&io_luaState, requiredStackSlotCount))
			{
				// Load the file and compile its contents into a Lua function
				const auto luaResult = luaL_loadfile(&io_luaState, s_userSettingsFileName);
				if (luaResult == LUA_OK)
				{
					// Set the Lua function's environment
					{
						// Push the empty table to the top of the stack
						lua_pushvalue(&io_luaState, -2);
						// Set the empty table as the function's global environment
						// (this means that anything that the file syntactically adds to the global environment
						// will actually be added to the table)
						constexpr int globalEnvironmentUpvalueIndex = 1;
						const auto* const upvalueName = lua_setupvalue(&io_luaState, -2, globalEnvironmentUpvalueIndex);
						if (upvalueName)
						{
							EAE6320_ASSERT(strcmp("_ENV", upvalueName) == 0);
						}
						else
						{
							EAE6320_ASSERT(false);
							eae6320::Logging::OutputError("Internal error setting the Lua environment for the user settings file \"%s\"!"
								" This should never happen", s_userSettingsFileName);
							lua_pop(&io_luaState, 2);
							return eae6320::Results::Failure;
						}
					}
					// Call the Lua function
					// (this will add anything that the file syntactically sets in the global environment
					// into the empty table that was created)
					{
						constexpr int noArguments = 0;
						constexpr int noReturnValues = 0;
						constexpr int noErrorMessageHandler = 0;
						const auto luaResult = lua_pcall(&io_luaState, noArguments, noReturnValues, noErrorMessageHandler);
						if (luaResult == LUA_OK)
						{
							return eae6320::Results::Success;
						}
						else
						{
							const std::string luaErrorMessage = lua_tostring(&io_luaState, -1);
							lua_pop(&io_luaState, 1);

							EAE6320_ASSERTF(false, "User settings file error: %s", luaErrorMessage.c_str());
							if (luaResult == LUA_ERRRUN)
							{
								eae6320::Logging::OutputError("Error in the user settings file \"%s\": %s",
									s_userSettingsFileName, luaErrorMessage);
							}
							else
							{
								eae6320::Logging::OutputError("Error processing the user settings file \"%s\": %s",
									s_userSettingsFileName, luaErrorMessage);
							}
							InitializeDefaultValues();
							return eae6320::Results::InvalidFile;
						}
					}
				}
				else
				{
					const std::string luaErrorMessage = lua_tostring(&io_luaState, -1);
					lua_pop(&io_luaState, 1);

					if (luaResult == LUA_ERRFILE)
					{
						EAE6320_ASSERTF(false, "Error opening or reading user settings file: %s", luaErrorMessage.c_str());
						eae6320::Logging::OutputError("Error opening or reading the user settings file \"%s\" even though it exists: %s",
							s_userSettingsFileName, luaErrorMessage.c_str());

					}
					else if (luaResult == LUA_ERRSYNTAX)
					{
						EAE6320_ASSERTF(false, "Syntax error in user settings file: %s", luaErrorMessage.c_str());
						eae6320::Logging::OutputError("Syntax error in the user settings file \"%s\": %s",
							s_userSettingsFileName, luaErrorMessage.c_str());
					}
					else
					{
						EAE6320_ASSERTF(false, "Error loading user settings file: %s", luaErrorMessage.c_str());
						eae6320::Logging::OutputError("Error loading the user settings file \"%s\": %s",
							s_userSettingsFileName, luaErrorMessage.c_str());
					}
					InitializeDefaultValues();
					return eae6320::Results::InvalidFile;
				}
			}
			else
			{
				EAE6320_ASSERTF(false, "Not enough stack space to load user settings file");
				eae6320::Logging::OutputError("Lua can't allocate enough stack space to load the user settings file \"%s\"",
					s_userSettingsFileName);
				InitializeDefaultValues();
				return eae6320::Results::OutOfMemory;
			}
		}
		else
		{
			// If loading the file failed because the file doesn't exist it's ok;
			// default values will be used
			eae6320::Logging::OutputMessage("The user settings file \"%s\" doesn't exist. Using default settings instead.",
				s_userSettingsFileName);
			InitializeDefaultValues();
			return eae6320::Results::FileDoesntExist;
		}
	}

	eae6320::cResult InitializeIfNecessary()
	{
		static eae6320::cResult isInitialized;
		if (isInitialized != eae6320::Results::Undefined)
		{
			return isInitialized;
		}

		auto result = eae6320::Results::Success;

		// Create a new Lua state
		lua_State* luaState = nullptr;
		auto wasUserSettingsEnvironmentCreated = false;

		const eae6320::cScopeCleanUp autoCleanUp([&]()
		{
			// Free the Lua environment
			if (luaState)
			{
				if (wasUserSettingsEnvironmentCreated)
				{
					lua_pop(luaState, 1);
				}
				EAE6320_ASSERTF(lua_gettop(luaState) == 0, "Lua stack is inconsistent");
				lua_close(luaState);
				luaState = nullptr;
			}
			// Store the result
			isInitialized = result;
		});

		int jumpValue = 0;	// 0 means no jump has happened
		{
			luaState = luaL_newstate();
			if (luaState)
			{
				// Set a function that will be called if Lua is about to abort
				lua_atpanic(luaState, OnLuaPanic);
			}
			else
			{
				result = eae6320::Results::OutOfMemory;
				EAE6320_ASSERTF(false, "Failed to create a new Lua state");
				eae6320::Logging::OutputError("Failed to create a new Lua state for the user settings");
				return result;
			}
		}
		// Set a restore point in case Lua panics
		jumpValue = setjmp(s_jumpBuffer);
		if (jumpValue == 0)
		{
			// Create an empty table to be used as the Lua environment for the user settings
			if (lua_checkstack(luaState, 1))
			{
				lua_newtable(luaState);
				wasUserSettingsEnvironmentCreated = true;
			}
			else
			{
				result = eae6320::Results::OutOfMemory;
				EAE6320_ASSERTF(false, "Lua didn't increase its tack for a new table");
				eae6320::Logging::OutputError("User settings files can't be processed"
					" because Lua can't increase its stack for a new table");
				return result;
			}
			// Load the user settings
			if (result = LoadUserSettingsIntoLuaTable(*luaState))
			{
				// Populate the user settings in C from the user settings in the Lua environment
				if (!(result = PopulateUserSettingsFromLuaTable(*luaState)))
				{
					return result;
				}
			}
			else
			{
				return result;
			}
		}
		else
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERTF(false, "Unhandled Lua error");
			eae6320::Logging::OutputError("User settings files can't be processed"
				" because of an unhandled Lua error");
		}

		return result;
	}
}

namespace
{
	eae6320::cResult LoadValuesFromBinary()
	{
		eae6320::cResult result = eae6320::Results::Success;
		eae6320::Platform::sDataFromFile dataFromFile;
		if (eae6320::Platform::LoadBinaryFile(s_controllerSettingsFileName, dataFromFile))
		{
			uintptr_t offset = reinterpret_cast<uintptr_t>(dataFromFile.data);
			uintptr_t maxOffset = offset + dataFromFile.size;
			for (uint8_t i = 0; i < 4; i++)
			{
				if (*(reinterpret_cast<uint8_t*>(offset)) == i)
				{
					offset += sizeof(uint8_t);
					g_KeyMapping[i][eae6320::UserInput::ControllerInput::ControllerKeyCodes::DPAD_UP] = *reinterpret_cast<uint16_t*>(offset);
					offset += sizeof(uint16_t);
					g_KeyMapping[i][eae6320::UserInput::ControllerInput::ControllerKeyCodes::DPAD_DOWN] = *reinterpret_cast<uint16_t*>(offset);
					offset += sizeof(uint16_t);
					g_KeyMapping[i][eae6320::UserInput::ControllerInput::ControllerKeyCodes::DPAD_LEFT] = *reinterpret_cast<uint16_t*>(offset);
					offset += sizeof(uint16_t);
					g_KeyMapping[i][eae6320::UserInput::ControllerInput::ControllerKeyCodes::DPAD_RIGHT] = *reinterpret_cast<uint16_t*>(offset);
					offset += sizeof(uint16_t);
					g_KeyMapping[i][eae6320::UserInput::ControllerInput::ControllerKeyCodes::START] = *reinterpret_cast<uint16_t*>(offset);
					offset += sizeof(uint16_t);
					g_KeyMapping[i][eae6320::UserInput::ControllerInput::ControllerKeyCodes::BACK] = *reinterpret_cast<uint16_t*>(offset);
					offset += sizeof(uint16_t);
					g_KeyMapping[i][eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_THUMB] = *reinterpret_cast<uint16_t*>(offset);
					offset += sizeof(uint16_t);
					g_KeyMapping[i][eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_THUMB] = *reinterpret_cast<uint16_t*>(offset);
					offset += sizeof(uint16_t);
					g_KeyMapping[i][eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_SHOULDER] = *reinterpret_cast<uint16_t*>(offset);
					offset += sizeof(uint16_t);
					g_KeyMapping[i][eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_SHOULDER] = *reinterpret_cast<uint16_t*>(offset);
					offset += sizeof(uint16_t);
					g_KeyMapping[i][eae6320::UserInput::ControllerInput::ControllerKeyCodes::A] = *reinterpret_cast<uint16_t*>(offset);
					offset += sizeof(uint16_t);
					g_KeyMapping[i][eae6320::UserInput::ControllerInput::ControllerKeyCodes::B] = *reinterpret_cast<uint16_t*>(offset);
					offset += sizeof(uint16_t);
					g_KeyMapping[i][eae6320::UserInput::ControllerInput::ControllerKeyCodes::X] = *reinterpret_cast<uint16_t*>(offset);
					offset += sizeof(uint16_t);
					g_KeyMapping[i][eae6320::UserInput::ControllerInput::ControllerKeyCodes::Y] = *reinterpret_cast<uint16_t*>(offset);
					offset += sizeof(uint16_t);
					if (!(*reinterpret_cast<uint16_t*>(offset) == 0))
					{
						offset += sizeof(uint16_t);
						g_DeadZones[i].push_back(*reinterpret_cast<uint16_t*>(offset));
						offset += sizeof(uint16_t);
					}
					else
					{
						g_DeadZones[i].push_back(0);
						offset += sizeof(uint16_t);
					}
					if (!(*reinterpret_cast<uint16_t*>(offset) == 0))
					{
						offset += sizeof(uint16_t);
						g_DeadZones[i].push_back(*reinterpret_cast<uint16_t*>(offset));
						offset += sizeof(uint16_t);
					}
					else
					{
						g_DeadZones[i].push_back(0);
						offset += sizeof(uint16_t);
					}
					if (!(*reinterpret_cast<uint16_t*>(offset) == 0))
					{
						offset += sizeof(uint16_t);
						g_DeadZones[i].push_back(*reinterpret_cast<uint16_t*>(offset));
						offset += sizeof(uint16_t);
					}
					else
					{
						g_DeadZones[i].push_back(0);
						offset += sizeof(uint16_t);
					}
					g_DefinedControllersInSettings++;
				}
			}
		}
		else
		{
			result = eae6320::Results::Failure;
		}
		return result;
	}
}

namespace
{
	eae6320::cResult LoadControllerSettings()
	{
		eae6320::cResult result = eae6320::Results::Success;
		if ((eae6320::Platform::DoesFileExist(s_controllerSettingsFileName)) || (eae6320::Platform::DoesFileExist(s_userSettingsFileName)))
		{
			if ((eae6320::Platform::DoesFileExist(s_controllerSettingsFileName)) && (eae6320::Platform::DoesFileExist(s_userSettingsFileName)))
			{
				uint64_t lastWriteBinary;
				uint64_t lastWriteLua;
				result = eae6320::Platform::GetLastWriteTime(s_controllerSettingsFileName, lastWriteBinary);
				result = eae6320::Platform::GetLastWriteTime(s_userSettingsFileName, lastWriteLua);
				if (lastWriteBinary > lastWriteLua)
				{
					result = LoadValuesFromBinary();
				}
				else
				{
					result = InitializeIfNecessary();
				}
			}
			else if (eae6320::Platform::DoesFileExist(s_controllerSettingsFileName))
			{
				result = LoadValuesFromBinary();
			}
			else if (eae6320::Platform::DoesFileExist(s_userSettingsFileName))
			{
				result = InitializeIfNecessary();
			}
		}
		else
		{
			InitializeDefaultValues();
		}
		if (!result)
		{
			InitializeDefaultValues();
		}
		return result;
	}
	//template<typename T, typename... U>
	//size_t GetAddress(std::function<T(U...)> f) {
	//	typedef T(fnType)(U...);
	//	fnType ** fnPointer = f.template target<fnType*>();
	//	return (size_t)*fnPointer;
	//}
}
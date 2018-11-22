// Includes
//=========

#include "cPrefabLoader.h"

#include <External/Lua/Includes.h>
#include <Tools/AssetBuildLibrary/Functions.h>
#include <Engine/Platform/Platform.h>
#include <iostream>
#include <Engine/Graphics/VertexFormats.h>

// Inherited Implementation
//=========================

// Build
//------

eae6320::cResult eae6320::Assets::cPrefabLoader::LoadAsset(const char* const i_path, std::string& io_meshPath, std::string& io_effectPath, bool& io_isActive)
{
	auto result = eae6320::Results::Success;

	// Create a new Lua state
	lua_State* luaState = nullptr;
	{
		luaState = luaL_newstate();
		if ( !luaState )
		{
			result = eae6320::Results::OutOfMemory;
			std::cerr << "Failed to create a new Lua state" << std::endl;
			return result;
 		}
	}

	// Load the asset file as a "chunk",
	// meaning there will be a callable function at the top of the stack
	const auto stackTopBeforeLoad = lua_gettop( luaState );
	{
		const auto luaResult = luaL_loadfile( luaState, i_path );
		if ( luaResult != LUA_OK )
		{
			result = eae6320::Results::Failure;
			std::cerr << lua_tostring( luaState, -1 ) << std::endl;
			// Pop the error message
			lua_pop( luaState, 1 );
			goto OnExit;
		}
	}
	// Execute the "chunk", which should load the asset
	// into a table at the top of the stack
	{
		constexpr int argumentCount = 0;
		constexpr int returnValueCount = LUA_MULTRET;	// Return _everything_ that the file returns
		constexpr int noMessageHandler = 0;
		const auto luaResult = lua_pcall( luaState, argumentCount, returnValueCount, noMessageHandler );
		if ( luaResult == LUA_OK )
		{
			// A well-behaved asset file will only return a single value
			const auto returnedValueCount = lua_gettop( luaState ) - stackTopBeforeLoad;
			if ( returnedValueCount == 1 )
			{
				// A correct asset file _must_ return a table
				if ( !lua_istable( luaState, -1 ) )
				{
					result = eae6320::Results::InvalidFile;
					std::cerr << "Asset files must return a table (instead of a "
						<< luaL_typename( luaState, -1 ) << ")" << std::endl;
					// Pop the returned non-table value
					lua_pop( luaState, 1 );
					goto OnExit;
				}
			}
			else
			{
				result = eae6320::Results::InvalidFile;
				std::cerr << "Asset files must return a single table (instead of "
					<< returnedValueCount << " values)" << std::endl;
				// Pop every value that was returned
				lua_pop( luaState, returnedValueCount );
				goto OnExit;
			}
		}
		else
		{
			result = eae6320::Results::InvalidFile;
			std::cerr << lua_tostring( luaState, -1 );
			// Pop the error message
			lua_pop( luaState, 1 );
			goto OnExit;
		}
	}

	// If this code is reached the asset file was loaded successfully,
	// and its table is now at index -1
	result = LoadTableValues( *luaState, io_meshPath, io_effectPath, io_isActive);

	// Pop the table
	lua_pop( luaState, 1 );

OnExit:

	if ( luaState )
	{
		// If I haven't made any mistakes
		// there shouldn't be anything on the stack,
		// regardless of any errors encountered while loading the file:
		EAE6320_ASSERT( lua_gettop( luaState ) == 0 );

		lua_close( luaState );
		luaState = nullptr;
	}

	return result;
}


eae6320::cResult eae6320::Assets::cPrefabLoader::LoadTableValues(lua_State& io_luaState, std::string& io_meshPath, std::string& io_effectPath, bool& io_isActive)
{
	auto result = eae6320::Results::Success;

	if ( !( result = LoadTableValues_isActive( io_luaState, io_isActive ) ) )
	{
		return result;
	}

	if ( !( result = LoadTableValues_componentArray( io_luaState, io_meshPath, io_effectPath) ) )
	{
		return result;
	}

	return result;
}

eae6320::cResult eae6320::Assets::cPrefabLoader::LoadTableValues_isActive(lua_State& io_luaState, bool& io_isActive)
{
	auto result = eae6320::Results::Success;

	auto* key = "isActive";
	lua_pushstring( &io_luaState, key );
	lua_gettable( &io_luaState, -2 );

	if ( lua_isboolean( &io_luaState, -1 ) )
	{
		const auto value = lua_toboolean( &io_luaState, -1 );
		io_isActive = static_cast<bool>(value);
		lua_pop( &io_luaState, 1 );
	}
	else
	{
		result = eae6320::Results::InvalidFile;
		std::cerr << "The value at \"" << key << "\" must be a table "
			"(instead of a " << luaL_typename( &io_luaState, -1 ) << ")" << std::endl;
		goto OnExit;
	}

OnExit:

	return result;
}
eae6320::cResult eae6320::Assets::cPrefabLoader::LoadTableValues_componentArray(lua_State& io_luaState, std::string& io_meshPath, std::string& io_effectPath)
{
	auto result = eae6320::Results::Success;

	// Right now the asset table is at -1.
	// After the following table operation it will be at -2
	// and the "textures" table will be at -1:
	constexpr auto* const key = "components";
	lua_pushstring( &io_luaState, key );
	lua_gettable( &io_luaState, -2 );
	// It can be hard to remember where the stack is at
	// and how many values to pop.
	// One strategy I would suggest is to always call a new function
	// When you are at a new level:
	// Right now we know that we have an original table at -2,
	// and a new one at -1,
	// and so we _know_ that we always have to pop at least _one_
	// value before leaving this function
	// (to make the original table be back to index -1).
	// If we don't do any further stack manipulation in this function
	// then it becomes easy to remember how many values to pop
	// because it will always be one.
	// This is the strategy I'll take in this example
	// (look at the "OnExit" label):
	if ( lua_istable( &io_luaState, -1 ) )
	{
		if ( !( result = LoadTableValues_components( io_luaState, io_meshPath, io_effectPath) ) )
		{
			goto OnExit;
		}
	}
	else
	{
		result = eae6320::Results::InvalidFile;
		std::cerr << "The value at \"" << key << "\" must be a table "
			"(instead of a " << luaL_typename( &io_luaState, -1 ) << ")" << std::endl;
		goto OnExit;
	}

OnExit:

	// Pop the textures table
	lua_pop( &io_luaState, 1 );

	return result;
}

eae6320::cResult eae6320::Assets::cPrefabLoader::LoadTableValues_components(lua_State& io_luaState, std::string& io_meshPath, std::string& io_effectPath)
{
	auto result = eae6320::Results::Success;
	const auto arrayLength = luaL_len( &io_luaState, -1 );
	if ( arrayLength > 0 )
	{
		std::cout << "Iterating through the ordered values:" << std::endl;
		// Remember that Lua arrays are 1-based and not 0-based!
		for ( int i = 1; i <= arrayLength; ++i )
		{
			lua_pushinteger( &io_luaState, i );
			lua_gettable( &io_luaState, -2 );
			if ( lua_istable( &io_luaState, -1 ) )
			{
				if ( !( result = LoadTableValues_componentValues( io_luaState, io_meshPath, io_effectPath) ) )
				{
					goto OnExit;
				}
				lua_pop( &io_luaState, 1 );
			}
			else
			{
				lua_pop( &io_luaState, 1 );
				goto OnExit;
			}
		}
	}
	else
	{
		result = eae6320::Results::InvalidFile;
		std::cerr << "The asset table doesn't have any ordered values" << std::endl;
		return result;
	}

OnExit:
	return result;
}

eae6320::cResult eae6320::Assets::cPrefabLoader::LoadTableValues_componentValues(lua_State& io_luaState, std::string& io_meshPath, std::string& io_effectPath)
{
	auto result = eae6320::Results::Success;
	
	bool isMeshPath = false;;
	auto* key = "class";
	lua_pushstring( &io_luaState, key );
	lua_gettable( &io_luaState, -2 );

	if ( lua_isstring( &io_luaState, -1 ) )
	{
		const auto value = lua_tostring( &io_luaState, -1 );
		if (*value == 'M')
		{
			isMeshPath = true;
		}
		lua_pop( &io_luaState, 1 );
	}
	else
	{
		result = eae6320::Results::InvalidFile;
		std::cerr << "The value at \"" << key << "\" must be a table "
			"(instead of a " << luaL_typename( &io_luaState, -1 ) << ")" << std::endl;
		goto OnExit;
	}

	key = "path";
	lua_pushstring( &io_luaState, key );
	lua_gettable( &io_luaState, -2 );

	if ( lua_isstring( &io_luaState, -1 ) )
	{
		const auto value = lua_tostring( &io_luaState, -1 );
		if (isMeshPath)
		{
			io_meshPath = static_cast<std::string>(value);
		}
		else
		{
			io_effectPath = static_cast<std::string>(value);
		}
		lua_pop( &io_luaState, 1 );
	}
	else
	{
		result = eae6320::Results::InvalidFile;
		std::cerr << "The value at \"" << key << "\" must be a table "
			"(instead of a " << luaL_typename( &io_luaState, -1 ) << ")" << std::endl;
		goto OnExit;
	}

OnExit:

	return result;
}

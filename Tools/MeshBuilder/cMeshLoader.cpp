// Includes
//=========

#include "cMeshLoader.h"

#include <External/Lua/Includes.h>
#include <Tools/AssetBuildLibrary/Functions.h>
#include <Engine/Platform/Platform.h>
#include <iostream>
#include <Engine/Graphics/VertexFormats.h>

// Inherited Implementation
//=========================

// Build
//------

eae6320::cResult eae6320::Assets::cMeshLoader::LoadAsset(const char* const i_path, std::vector<Graphics::VertexFormats::sMesh>& io_vertexData, std::vector<uint16_t>& io_indexData)
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
	result = LoadTableValues( *luaState, io_vertexData, io_indexData );

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

eae6320::cResult eae6320::Assets::cMeshLoader::LoadTableValues(lua_State& io_luaState, std::vector<Graphics::VertexFormats::sMesh>& io_vertexData, std::vector<uint16_t>& io_indexData)
{
	auto result = eae6320::Results::Success;

	if ( !( result = LoadTableValues_vertexArray( io_luaState, io_vertexData ) ) )
	{
		return result;
	}

	if ( !( result = LoadTableValues_indexArray( io_luaState, io_indexData ) ) )
	{
		return result;
	}

	return result;
}

eae6320::cResult eae6320::Assets::cMeshLoader::LoadTableValues_vertexArray(lua_State& io_luaState, std::vector<Graphics::VertexFormats::sMesh>& io_vertexData)
{
	auto result = eae6320::Results::Success;

	// Right now the asset table is at -1.
	// After the following table operation it will be at -2
	// and the "textures" table will be at -1:
	constexpr auto* const key = "vertexArray";
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
		if ( !( result = LoadTableValues_vertexArray_vertices( io_luaState, io_vertexData ) ) )
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

eae6320::cResult eae6320::Assets::cMeshLoader::LoadTableValues_vertexArray_vertices(lua_State& io_luaState, std::vector<Graphics::VertexFormats::sMesh>& io_vertexData)
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
				if ( !( result = LoadTableValues_vertexArray_vertices_position( io_luaState, io_vertexData ) ) )
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

eae6320::cResult eae6320::Assets::cMeshLoader::LoadTableValues_vertexArray_vertices_position(lua_State& io_luaState, std::vector<Graphics::VertexFormats::sMesh>& io_vertexData)
{
	auto result = eae6320::Results::Success;

	// Right now the asset table is at -1.
	// After the following table operation it will be at -2
	// and the "textures" table will be at -1:
	constexpr auto* const key = "position";
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
		if ( !( result = LoadTableValues_vertexArray_vertices_position_values( io_luaState, io_vertexData ) ) )
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

eae6320::cResult eae6320::Assets::cMeshLoader::LoadTableValues_vertexArray_vertices_position_values(lua_State& io_luaState, std::vector<Graphics::VertexFormats::sMesh>& io_vertexData)
{
	auto result = eae6320::Results::Success;

	std::cout << "Iterating through every position value:" << std::endl;
	Graphics::VertexFormats::sMesh position;
	const auto valueCount = luaL_len( &io_luaState, -1 );
	for ( int i = 1; i <= valueCount; ++i )
	{
		lua_pushinteger( &io_luaState, i );
		lua_gettable( &io_luaState, -2 );
		const auto value = lua_tonumber( &io_luaState, -1 );

		if (i == 1)
		{
			position.x = static_cast<float>(value);
		}
		else if (i == 2)
		{
			position.y = static_cast<float>(value);
		}
		else if (i == 3)
		{
			position.z = static_cast<float>(value);
		}
		lua_pop( &io_luaState, 1 );
	}

	io_vertexData.push_back(position);

	return result;
}

eae6320::cResult eae6320::Assets::cMeshLoader::LoadTableValues_indexArray(lua_State& io_luaState, std::vector<uint16_t>& io_indexData)
{
	auto result = eae6320::Results::Success;

	// Right now the asset table is at -1.
	// After the following table operation it will be at -2
	// and the "textures" table will be at -1:
	constexpr auto* const key = "indexArray";
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
		if ( !( result = LoadTableValues_indexArray_indices( io_luaState, io_indexData ) ) )
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

eae6320::cResult eae6320::Assets::cMeshLoader::LoadTableValues_indexArray_indices(lua_State& io_luaState, std::vector<uint16_t>& io_indexData)
{
	auto result = eae6320::Results::Success;

	std::cout << "Iterating through every index:" << std::endl;
	const auto indexCount = luaL_len( &io_luaState, -1 );
	for ( int i = 1; i <= indexCount; ++i )
	{
		lua_pushinteger( &io_luaState, i );
		lua_gettable( &io_luaState, -2 );
		const auto value = lua_tonumber( &io_luaState, -1 );
		io_indexData.push_back(static_cast<uint16_t>(value));
		lua_pop( &io_luaState, 1 );
	}

	return result;
}


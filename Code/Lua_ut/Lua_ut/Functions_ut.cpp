#include <Catch2/catch.hpp>

#include <lua/lua.hpp>

#include <iostream>

// https://www.youtube.com/playlist?list=PLLwK93hM93Z3nhfJyRRWGRXHaXgNX0Itk

namespace
{
	int MyCppFunction(lua_State* L)
	{
		lua_Number a = lua_tonumber(L, -2);
		lua_Number b = lua_tonumber(L, -1);

		lua_pushnumber(L, a);
		lua_pushnumber(L, b);
		lua_pushnumber(L, 3);
		return 3;
	}
}

TEST_CASE("lua::CppToLua")
{
	constexpr const char* LUA_FILE = R"(
		function MyLuaFunction(a)
			return a, 2
		end
	)";

	lua_State* L = luaL_newstate();
	luaL_dostring(L, LUA_FILE);
	lua_getglobal(L, "MyLuaFunction");

	if (lua_isfunction(L, -1))
	{
		constexpr int NUM_ARGS = 1;
		constexpr int NUM_RET = 2;

		lua_pushnumber(L, 1);

		lua_pcall(L, NUM_ARGS, NUM_RET, 0);
		lua_Number a = lua_tonumber(L, -2);
		lua_Number b = lua_tonumber(L, -1);

		CHECK(int(a) == 1);
		CHECK(int(b) == 2);
	}
	lua_close(L);
}

TEST_CASE("lua::LuaToCpp")
{
	constexpr const char* LUA_FILE = R"(
		function MyLuaFunction(a)
			return MyCppFunction(a, 2)
		end
	)";

	lua_State* L = luaL_newstate();
	lua_pushcfunction(L, MyCppFunction);
	lua_setglobal(L, "MyCppFunction");

	luaL_dostring(L, LUA_FILE);
	lua_getglobal(L, "MyLuaFunction");

	if (lua_isfunction(L, -1))
	{
		constexpr int NUM_ARGS = 1;
		constexpr int NUM_RET = 3;

		lua_pushnumber(L, 1);

		lua_pcall(L, NUM_ARGS, NUM_RET, 0);
		lua_Number a = lua_tonumber(L, -3);
		lua_Number b = lua_tonumber(L, -2);
		lua_Number c = lua_tonumber(L, -1);

		CHECK(int(a) == 1);
		CHECK(int(b) == 2);
		CHECK(int(c) == 3);
	}
	lua_close(L);
}
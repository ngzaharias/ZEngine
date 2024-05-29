
#include <lua/lua.hpp>

#include <iostream>

int MyCppFunction(lua_State* L)
{
	lua_Number a = lua_tonumber(L, -2);
	lua_Number b = lua_tonumber(L, -1);

	lua_pushnumber(L, a);
	lua_pushnumber(L, b);
	lua_pushnumber(L, 3);
	return 3;
}

// https://www.youtube.com/playlist?list=PLLwK93hM93Z3nhfJyRRWGRXHaXgNX0Itk

int main(int argc, char* argv[])
{
	lua_State* L = luaL_newstate();
	lua_pushcfunction(L, MyCppFunction);
	lua_setglobal(L, "MyCppFunction");
	luaL_dofile(L, "D:/Projects/C++/ZEngine/Code/Lua_ut/Lua_ut/main.lua");
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

		printf("a = %d, b = %d, c = %d\n", (int)a, (int)b, (int)c);
	}
	lua_close(L);

	return 0;
}
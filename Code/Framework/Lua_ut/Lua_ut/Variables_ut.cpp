#include <Catch2/catch.hpp>

#include <lua/lua.hpp>

// https://www.youtube.com/playlist?list=PLLwK93hM93Z3nhfJyRRWGRXHaXgNX0Itk

TEST_CASE("lua::Variable")
{
	constexpr const char* LUA_FILE = R"(
		x = 42
	)";

	lua_State* L = luaL_newstate();
	luaL_dostring(L, LUA_FILE);

	lua_getglobal(L, "x");
	lua_Number a = lua_tonumber(L, -1);
	CHECK(int(a) == 42);
}
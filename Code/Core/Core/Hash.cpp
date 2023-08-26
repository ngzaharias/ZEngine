#include "Core/Hash.h"

#include <entt/core/hashed_string.hpp>

// #todo: add str::StringView version

str::Hash str::ToHash(const char* value)
{
	return entt::hashed_string(value);
}

str::Hash str::ToHash(const str::String& value)
{
	return entt::hashed_string(value.c_str());
}
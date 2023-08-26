#pragma once

#define TOML_EXCEPTIONS 0
#include <tomlplusplus/toml.hpp>

namespace toml
{
	using Array = toml::array;
	using Node = toml::node;
	using Value = toml::table;
	using Table = toml::table;
}

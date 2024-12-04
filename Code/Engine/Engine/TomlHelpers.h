#pragma once

#include "Core/Path.h"
#include "Engine/TomlTypes.h"

namespace toml
{
	/// \brief
	bool LoadDocument(const str::Path& filepath, toml::Table& out_Document);

	/// \brief
	bool SaveDocument(const str::Path& filepath, const toml::Table& in_Document);

	/// \brief Parses a toml::Value into a specific type and returns it.
	/// If the parsing fails for any reason the _default value will be returned instead.
	template<typename Type>
	Type Parse(const toml::Node& value, const Type& _default);

	/// \brief Parses a toml::Value into a specific type and returns it.
	/// If the parsing fails for any reason the _default value will be returned instead.
	template<typename Type>
	Type Parse(const toml::Value& value, const str::StringView& member, const Type& _default);
}

#include "TomlHelpers.inl"
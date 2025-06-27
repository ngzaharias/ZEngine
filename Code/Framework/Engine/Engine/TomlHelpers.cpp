#include "EnginePCH.h"
#include "Engine/TomlHelpers.h"

bool toml::SaveDocument(const str::Path& filepath, const toml::Table& in_Document)
{
	std::ofstream file;
	file.open(filepath.ToChar(), std::fstream::out);
	if (!file.is_open())
		return false;

	file << in_Document;
	return true;
}

bool toml::LoadDocument(const str::Path& filepath, toml::Table& out_Document)
{
	toml::parse_result result = toml::parse_file(filepath.ToView());
	if (!result)
		return false;

	out_Document = std::move(result).table();
	return true;
}
#include "Core/Hash.h"

#include "Core/StringView.h"

namespace
{
	// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
	uint32 ToU32(const str::StringView& value)
	{
		constexpr uint32 offset = 2166136261;
		constexpr uint32 prime = 16777619;

		uint32 hash = offset;
		for (const char v : value)
			hash = (hash ^ static_cast<uint32>(v)) * prime;
		return hash;
	}

	uint64 ToU64(const str::StringView& value)
	{
		constexpr uint64 offset = 14695981039346656037ull;
		constexpr uint64 prime = 1099511628211ull;

		uint64 hash = offset;
		for (const char v : value)
			hash = (hash ^ static_cast<uint32>(v)) * prime;
		return hash;
	}
}

str::Hash str::ToHash(const str::StringView& value)
{
	return ToU32(value);
}
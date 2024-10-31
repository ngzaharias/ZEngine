#pragma once

#include "Core/StringView.h"

namespace str
{
	using uint32 = uint32_t;
	using uint64 = uint64_t;

	using Hash = uint32;
	using Hash32 = uint32;
	using Hash64 = uint64;

	constexpr str::Hash32 ToHash32(const str::StringView& value)
	{
		constexpr uint32 offset = 2166136261;
		constexpr uint32 prime = 16777619;

		uint32 hash = offset;
		for (const char v : value)
			hash = (hash ^ static_cast<uint32>(v)) * prime;
		return hash;
	}

	constexpr str::Hash64 ToHash64(const str::StringView& value)
	{
		constexpr uint64 offset = 14695981039346656037ull;
		constexpr uint64 prime = 1099511628211ull;

		uint64 hash = offset;
		for (const char v : value)
			hash = (hash ^ static_cast<uint32>(v)) * prime;
		return hash;
	}

	// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
	constexpr str::Hash32 ToHash(const str::StringView& value)
	{
		return ToHash32(value);
	}
}
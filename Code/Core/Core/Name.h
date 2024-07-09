#pragma once

#include "Core/Hash.h"
#include "Core/String.h"
#include "Core/StringView.h"

#define NAME(string) str::Name::Create(string)

namespace str
{
	class Name final
	{
	public:
		static Name Unassigned;

		bool operator==(const str::Name& rhs) const { return m_Hash == rhs.m_Hash; }
		bool operator!=(const str::Name& rhs) const { return m_Hash != rhs.m_Hash; }
		bool operator<(const str::Name& rhs) const { return m_Hash < rhs.m_Hash; }

		operator str::StringView() const;

		const bool IsEmpty() const;

		const char* ToChar() const;
		const str::Hash ToHash() const { return m_Hash; }
		const str::String& ToString() const;

		static str::Name Create(const str::StringView& string);

	private:
		str::Hash m_Hash = UINT32_MAX;

#ifdef Z_DEBUG
		const str::String* m_Debug = nullptr;
#endif
	};
}
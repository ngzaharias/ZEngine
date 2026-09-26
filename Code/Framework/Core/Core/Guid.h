#pragma once

#include "Core/String.h"
#include "Core/StringView.h"
#include "Core/Types.h"

#define GUID(string) str::Guid::Create(string)
#define GUID64(value) str::Guid::Create(value)

namespace str
{
	class Guid final
	{
		union Data
		{
			uint64 m_U64[2];
			uint32 m_U32[4];
			uint16 m_U16[8];
			uint8  m_U8[16];
		};

	public:
		static str::Guid Unassigned;

		str::Guid() { m_Data.m_U64[0] = 0; m_Data.m_U64[1] = 0; }
		str::Guid(const uint64 (&value)[2]) { m_Data.m_U64[0] = value[0]; m_Data.m_U64[1] = value[1]; }

		str::String ToString() const;

		inline bool IsValid() const { return m_Data.m_U64[0] != 0 || m_Data.m_U64[1] != 0; }

		inline bool operator<(const str::Guid& rhs) const { return (m_Data.m_U64[0] != rhs.m_Data.m_U64[0]) ? (m_Data.m_U64[0] < rhs.m_Data.m_U64[0]) : (m_Data.m_U64[1] < rhs.m_Data.m_U64[1]); }
		inline bool operator==(str::Guid const& rhs) const { return m_Data.m_U64[0] == rhs.m_Data.m_U64[0] && m_Data.m_U64[1] == rhs.m_Data.m_U64[1]; }
		inline bool operator!=(str::Guid const& rhs) const { return m_Data.m_U64[0] != rhs.m_Data.m_U64[0] || m_Data.m_U64[1] != rhs.m_Data.m_U64[1]; }

		static str::Guid Create(const uint64 value);
		static str::Guid Create(const str::StringView& string);

		static str::Guid Generate();

		static bool IsValidString(const str::StringView& string);

		static void ValidateString(const str::StringView& string);

	public:
		Data m_Data;
	};
}

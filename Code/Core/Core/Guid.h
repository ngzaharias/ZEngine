#pragma once

#include "Core/String.h"
#include "Core/StringView.h"
#include "Core/Types.h"

#define GUID(string) str::Guid::Create(string)

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
		static Guid Unassigned;

		Guid() { m_Data.m_U64[0] = 0; m_Data.m_U64[1] = 0; }
		Guid(const uint64 (&value)[2]) { m_Data.m_U64[0] = value[0]; m_Data.m_U64[1] = value[1]; }

		str::String ToString() const;

		inline bool IsValid() const { return m_Data.m_U64[0] != 0 || m_Data.m_U64[1] != 0; }

		inline bool operator<(Guid const& rhs) const { return m_Data.m_U64[0] < rhs.m_Data.m_U64[0] || (m_Data.m_U64[0] == rhs.m_Data.m_U64[0] && m_Data.m_U64[1] < rhs.m_Data.m_U64[1]); }
		inline bool operator==(Guid const& rhs) const { return m_Data.m_U64[0] == rhs.m_Data.m_U64[0] && m_Data.m_U64[1] == rhs.m_Data.m_U64[1]; }
		inline bool operator!=(Guid const& rhs) const { return m_Data.m_U64[0] != rhs.m_Data.m_U64[0] || m_Data.m_U64[1] != rhs.m_Data.m_U64[1]; }

		static Guid Create(const str::StringView& string);
		static Guid Generate();
		static bool IsValidString(const str::StringView& string);

#ifdef ASSERTS_ENABLED
		static void ValidateString(const str::StringView& string);
#endif // ASSERTS_ENABLED

	public:
		Data m_Data;
	};
}

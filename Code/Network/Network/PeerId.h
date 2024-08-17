#pragma once

#include "Core/Types.h"

namespace net
{
	/// \brief Represents a single computer that is connected a server.
	/// Note: Only the first 16 bits are used.
	struct PeerId
	{
		static const PeerId Unassigned;

		constexpr PeerId() = default;
		constexpr PeerId(const int32 value) : m_Value(value) { }
		constexpr PeerId(const PeerId& peerId) : m_Value(peerId.m_Value) { }

		constexpr int32 GetIndex() const { return m_Value; }

		constexpr bool operator==(const PeerId& rhs) const { return m_Value == rhs.m_Value; }
		constexpr bool operator<(const PeerId& rhs) const { return m_Value < rhs.m_Value; }

		int32 m_Value = INT32_MAX;
	};
}

inline constexpr net::PeerId net::PeerId::Unassigned(INT32_MAX);
#pragma once

#include "Core/Types.h"

namespace net
{
	/// \brief Represents a single computer that is connected to a host.
	struct PeerId
	{
		// Value reserved for the host.
		static const PeerId HostId;
		static const PeerId Unassigned;

		constexpr PeerId() = default;
		constexpr PeerId(const int32 value) : m_Value(value) { }

		constexpr int32 GetIndex() const { return m_Value; }

		constexpr bool operator==(const PeerId& rhs) const { return m_Value == rhs.m_Value; }
		constexpr bool operator<(const PeerId& rhs) const { return m_Value < rhs.m_Value; }

		int32 m_Value = INT32_MAX;
	};
}

inline constexpr net::PeerId net::PeerId::HostId(0);
inline constexpr net::PeerId net::PeerId::Unassigned;

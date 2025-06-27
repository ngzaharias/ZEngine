#pragma once

#include "Core/Types.h"

#include "Network/PeerId.h"

namespace net
{
	constexpr int32 s_InputMask = 0x0000FFFF;
	constexpr int32 s_PeerMask = 0x0000FFFF;
	constexpr int32 s_PeerOffset = 16;

	/// \brief Represents a single input that is connected to a computer.
	/// Note: Only the first 16 bits are used.
	using InputId = int32;

	/// \brief Represents a single input that is connected to a server through a peer (computer).
	struct UserId
	{
		static const UserId Unassigned;

		UserId() = default;
		UserId(const net::InputId& inputId, const net::PeerId& peerId)
		{
			const int32 a = (inputId & s_InputMask);
			const int32 b = (peerId.m_Value & s_PeerMask) << s_PeerOffset;
			m_Value = a + b;
		}
		UserId(const int32 value) : m_Value(value) { }
		UserId(const UserId& userId) : m_Value(userId.m_Value) { }

		int32 GetIndex() const { return m_Value; }
		net::InputId GetInputId() const
		{
			return m_Value & s_PeerMask;
		}
		net::PeerId GetPeerId() const
		{
			return net::PeerId((m_Value >> s_PeerOffset) & s_PeerMask);
		}

		bool operator==(const UserId& rhs) const { return m_Value == rhs.m_Value; }
		bool operator<(const UserId& rhs) const { return m_Value < rhs.m_Value; }

		int32 m_Value = INT16_MAX;
	};
}

inline constexpr net::UserId net::UserId::Unassigned(INT32_MAX);
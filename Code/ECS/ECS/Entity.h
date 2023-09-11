#pragma once

using uint32 = uint32_t;
using uint64 = uint64_t;

namespace ecs
{
	constexpr uint64 s_IndexMask = 0x00000000FFFFFFFF;
	constexpr uint64 s_VersionMask = 0x00000000FFFFFFFF;
	constexpr uint64 s_VersionOffset = 32;

	struct Entity
	{
		static const Entity Unassigned;

		Entity() = default;
		Entity(const uint32 index, const uint32 version) 
		{ 
			const uint64 a = (index & s_IndexMask);
			const uint64 b = (version & s_VersionMask) << s_VersionOffset;
			m_Value = a + b;
		}
		Entity(const uint64& value) : m_Value(value) { }
		Entity(const Entity& entity) : m_Value(entity.m_Value) { }

		bool operator==(const Entity& rhs) const { return m_Value == rhs.m_Value; }
		bool operator!=(const Entity& rhs) const { return !this->operator==(rhs); }
		bool operator<(const Entity& rhs) const { return m_Value < rhs.m_Value; }

		bool IsUnassigned() const
		{
			return m_Value == UINT64_MAX;
		}

		int32 GetIndex() const
		{
			const uint64 index = (m_Value & s_IndexMask);
			return static_cast<int32>(index);
		}

		int32 GetVersion() const
		{
			const uint64 version = (m_Value >> s_VersionOffset) & s_VersionMask;
			return static_cast<int32>(version);
		}

		uint64 m_Value = UINT64_MAX;
	};
}
inline constexpr ecs::Entity ecs::Entity::Unassigned;
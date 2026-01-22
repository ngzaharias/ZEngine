#pragma once

#include "Core/Types.h"

namespace ecs
{
	constexpr uint64 s_IndexMask = 0x00000000FFFFFFFF;
	constexpr uint64 s_IndexOffset = 0;
	constexpr uint64 s_VersionMask = 0x0000000000FFFFFF;
	constexpr uint64 s_VersionOffset = 32;
	constexpr uint64 s_OwnershipMask = 0x00000000000000FF;
	constexpr uint64 s_OwnershipOffset = 54;

	struct Entity
	{
		static const uint8 MainId;
		static const uint8 SyncId;
		static const Entity Unassigned;

		Entity() = default;
		Entity(const uint64& value) : m_Value(value) {}
		Entity(const Entity& entity) : m_Value(entity.m_Value) {}

		Entity(const uint32 index, const uint32 version) 
		{ 
			const uint64 a = (index & s_IndexMask);
			const uint64 b = (version & s_VersionMask) << s_VersionOffset;
			m_Value = a | b;
		}

		Entity(const uint32 index, const uint32 version, const uint8 ownership)
		{
			const uint64 a = (index & s_IndexMask) << s_IndexOffset;
			const uint64 b = (version & s_VersionMask) << s_VersionOffset;
			const uint64 c = (ownership & s_OwnershipMask) << s_OwnershipOffset;
			m_Value = a | b | c;
		}

		operator bool() const { return !IsUnassigned(); }

		bool operator==(const Entity& rhs) const { return m_Value == rhs.m_Value; }
		bool operator!=(const Entity& rhs) const { return !this->operator==(rhs); }
		bool operator<(const Entity& rhs) const { return m_Value < rhs.m_Value; }

		bool IsUnassigned() const
		{
			return *this == Unassigned;
		}

		int32 GetIndex() const
		{
			const uint64 index = (m_Value >> s_IndexOffset) & s_IndexMask;
			return static_cast<int32>(index);
		}

		int32 GetVersion() const
		{
			const uint64 version = (m_Value >> s_VersionOffset) & s_VersionMask;
			return static_cast<int32>(version);
		}

		int32 GetOwnership() const
		{
			const uint64 ownership = (m_Value >> s_OwnershipOffset) & s_OwnershipMask;
			return static_cast<int32>(ownership);
		}

		uint64 m_Value = 0;
	};
}

inline constexpr uint8 ecs::Entity::MainId = 0;
inline constexpr uint8 ecs::Entity::SyncId = 1;
inline constexpr ecs::Entity ecs::Entity::Unassigned;
#pragma once

#include "Core/Types.h"

namespace net
{
	struct Entity
	{
		static const Entity Unassigned;

		Entity() = default;
		Entity(const uint64& value) : m_Value(value) { }
		Entity(const Entity& entity) : m_Value(entity.m_Value) { }

		bool IsUnassigned() const;

		bool operator==(const Entity& rhs) const { return m_Value == rhs.m_Value; }
		bool operator!=(const Entity& rhs) const { return !this->operator==(rhs); }
		bool operator<(const Entity& rhs) const { return m_Value < rhs.m_Value; }

		uint64 m_Value = UINT64_MAX;
	};
}
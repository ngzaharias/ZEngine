#pragma once

#include "ECS/ComponentMask.h"

namespace ecs
{
	class EntityBuffer;
	class TypeRegistry;
}

namespace ecs
{
	struct EntityLayout
	{
		bool operator<(const ecs::EntityLayout& rhs) const
		{
			return (m_AddedMask != rhs.m_AddedMask)
				? (m_AddedMask   < rhs.m_AddedMask)   : (m_RemovedMask != rhs.m_RemovedMask)
				? (m_RemovedMask < rhs.m_RemovedMask) : (m_IncludeMask != rhs.m_IncludeMask)
				? (m_IncludeMask < rhs.m_IncludeMask) : (m_IsDead < rhs.m_IsDead);
		}

		ecs::ComponentMask GetMask() const { return m_AddedMask | m_RemovedMask | m_IncludeMask; };

		ecs::ComponentMask m_AddedMask = {};
		ecs::ComponentMask m_RemovedMask = {};
		ecs::ComponentMask m_IncludeMask = {};
		bool m_IsDead = false;
	};
}

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
			return (m_IncludeMask != rhs.m_IncludeMask) 
				? (m_IncludeMask < rhs.m_IncludeMask) : (m_RemovedMask != rhs.m_RemovedMask) 
				? (m_RemovedMask < rhs.m_RemovedMask) : (m_IsDead < rhs.m_IsDead);
		}

		ecs::ComponentMask m_IncludeMask = {};
		ecs::ComponentMask m_RemovedMask = {};
		bool m_IsDead = false;
	};
}

#pragma once

#include "ECS/ComponentMask.h"

namespace ecs
{
	struct QueryMask
	{
		// Mask of components that are stored externally of the entity table.
		ecs::ComponentMask m_ExternIncludeMask = {};

		// Mask of components that must have been added in the previous frame.
		ecs::ComponentMask m_TableAddedMask = {};
		// Mask of components that must have been removed in the previous frame.
		ecs::ComponentMask m_TableRemovedMask = {};
		// Mask of components that must have been updated in the previous frame.
		ecs::ComponentMask m_TableUpdatedMask = {};
		// Mask of components that must be present on the entity.
		ecs::ComponentMask m_TableIncludeMask = {};
		// Mask of components that cannot be present on the entity.
		ecs::ComponentMask m_TableExcludeMask = {};

		
		bool m_ConditionAlive = true;
		bool m_ConditionDead = false;
	};
}
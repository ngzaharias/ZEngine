#pragma once

#include "ECS/ComponentMask.h"

namespace ecs
{
	struct QueryMask
	{
		ecs::ComponentMask m_AddedMask = { };
		ecs::ComponentMask m_RemovedMask = { };
		ecs::ComponentMask m_UpdatedMask = { };
		ecs::ComponentMask m_IncludeMask = { };
		ecs::ComponentMask m_ExcludeMask = { };
	};
}
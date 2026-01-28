#pragma once

#include "ECS/ComponentMask.h"

namespace ecs
{
	struct EntityChange
	{
		ecs::ComponentMask m_Added;
		ecs::ComponentMask m_Removed;
		ecs::ComponentMask m_Updated;
		bool m_IsDestroy = false;
	};
}

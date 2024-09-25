#pragma once

#include "ECS/Component.h"

namespace dbg::hidden
{
	struct DebugComponent : ecs::Component<DebugComponent>
	{ 
		ecs::Entity m_Child = {};
	};
}
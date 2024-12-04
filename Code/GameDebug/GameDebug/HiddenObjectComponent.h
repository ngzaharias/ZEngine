#pragma once

#include "ECS/Component.h"

namespace dbg::hidden
{
	struct ObjectComponent : ecs::Component<ObjectComponent>
	{ 
		ecs::Entity m_Child = {};
	};
}
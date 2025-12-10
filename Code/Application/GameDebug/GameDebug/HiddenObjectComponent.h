#pragma once

#include "ECS/Component.h"

namespace debug::hidden
{
	struct ObjectComponent final : public ecs::Component<ObjectComponent>
	{ 
		ecs::Entity m_Child = {};
	};
}
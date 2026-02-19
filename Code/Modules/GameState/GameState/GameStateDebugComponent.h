#pragma once

#include "ECS/Component.h"

namespace gamestate
{
	struct DebugComponent final : public ecs::Component<DebugComponent> { };
};
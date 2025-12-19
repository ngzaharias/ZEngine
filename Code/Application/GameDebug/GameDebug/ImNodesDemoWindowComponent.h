#pragma once

#include "ECS/Component.h"

namespace debug
{
	struct ImNodesDemoWindowComponent final : public ecs::Component<ImNodesDemoWindowComponent> { };
};
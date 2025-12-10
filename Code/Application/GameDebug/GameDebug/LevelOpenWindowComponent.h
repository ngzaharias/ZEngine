#pragma once

#include "ECS/Component.h"

namespace debug::level
{
	struct OpenWindowComponent final : public ecs::Component<OpenWindowComponent> { };
}
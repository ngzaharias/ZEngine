#pragma once

#include "ECS/Component.h"

namespace dbg::level
{
	struct OpenRequest final : public ecs::Event<OpenRequest> { };

	struct WindowComponent final : public ecs::Component<WindowComponent> { };
}
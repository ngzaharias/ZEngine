#pragma once

#include "ECS/Component.h"

namespace dbg::level
{
	struct OpenRequestComponent final : public ecs::EventComponent<OpenRequestComponent> { };

	struct WindowComponent final : public ecs::Component<WindowComponent> { };
}
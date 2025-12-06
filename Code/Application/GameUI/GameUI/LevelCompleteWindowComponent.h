#pragma once

#include "ECS/Component.h"

namespace gui::level_complete
{
	struct WindowComponent final : public ecs::Component<WindowComponent> { };
}
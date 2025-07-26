#pragma once

#include "ECS/Component.h"

namespace gui::level_complete
{
	struct WindowComponent final : public ecs::Component<WindowComponent> { };

	struct ExitGameRequest final : public ecs::EventComponent<ExitGameRequest> { };
	struct ExitToMenuRequest final : public ecs::EventComponent<ExitToMenuRequest> { };
}
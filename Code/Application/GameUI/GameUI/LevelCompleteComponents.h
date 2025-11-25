#pragma once

#include "ECS/Component.h"

namespace gui::level_complete
{
	struct WindowComponent final : public ecs::Component<WindowComponent> { };

	struct ExitGameRequest final : public ecs::Event<ExitGameRequest> { };
	struct ExitToMenuRequest final : public ecs::Event<ExitToMenuRequest> { };
	struct ResetGameRequest final : public ecs::Event<ResetGameRequest> { };
}
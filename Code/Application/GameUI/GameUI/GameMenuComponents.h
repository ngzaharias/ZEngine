#pragma once

#include "ECS/Component.h"

namespace gui::game_menu
{
	struct WindowComponent final : public ecs::Component<WindowComponent> { };

	struct CloseRequest final : public ecs::Event<CloseRequest> { };
	struct ExitGameRequest final : public ecs::Event<ExitGameRequest> { };
	struct ExitToMenuRequest final : public ecs::Event<ExitToMenuRequest> { };
	struct OpenRequest final : public ecs::Event<OpenRequest> { };
}
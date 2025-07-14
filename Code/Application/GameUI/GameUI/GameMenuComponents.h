#pragma once

#include "ECS/Component.h"

namespace gui::game_menu
{
	struct WindowComponent final : public ecs::Component<WindowComponent> { };

	struct CloseRequest final : public ecs::EventComponent<CloseRequest> { };
	struct ExitGameRequest final : public ecs::EventComponent<ExitGameRequest> { };
	struct ExitToMenuRequest final : public ecs::EventComponent<ExitToMenuRequest> { };
	struct OpenRequest final : public ecs::EventComponent<OpenRequest> { };
	struct SettingsRequest final : public ecs::EventComponent<SettingsRequest> { };
}
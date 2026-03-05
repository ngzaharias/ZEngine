#pragma once

#include "ECS/Event.h"

namespace gui::settings_menu
{
	struct CloseRequestEvent final : public ecs::Event<CloseRequestEvent> { };
}
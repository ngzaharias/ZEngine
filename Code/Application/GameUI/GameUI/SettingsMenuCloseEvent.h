#pragma once

#include "ECS/Event.h"

namespace gui::settings_menu
{
	struct CloseEvent final : public ecs::Event<CloseEvent> { };
}
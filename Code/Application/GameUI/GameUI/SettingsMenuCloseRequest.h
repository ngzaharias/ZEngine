#pragma once

#include "ECS/Event.h"

namespace gui::settings_menu
{
	struct CloseRequest final : public ecs::Event<CloseRequest> { };
}
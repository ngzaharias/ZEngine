#pragma once

#include "ECS/Event.h"

namespace gui::settings_menu
{
	struct OpenRequest final : public ecs::Event<OpenRequest> { };
}
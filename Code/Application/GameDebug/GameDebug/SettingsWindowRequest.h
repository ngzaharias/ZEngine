#pragma once

#include "ECS/Event.h"

namespace dbg::settings
{
	struct WindowRequest final : public ecs::Event<WindowRequest> { };
}
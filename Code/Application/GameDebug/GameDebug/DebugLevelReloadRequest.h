#pragma once

#include "ECS/Event.h"

namespace debug::level
{
	struct ReloadRequest final : public ecs::Event<ReloadRequest> { };
}
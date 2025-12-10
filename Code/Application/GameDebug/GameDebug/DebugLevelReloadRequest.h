#pragma once

#include "ECS/Event.h"

namespace dbg::level
{
	struct ReloadRequest final : public ecs::Event<ReloadRequest> { };
}
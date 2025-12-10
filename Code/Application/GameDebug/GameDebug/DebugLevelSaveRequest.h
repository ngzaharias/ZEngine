#pragma once

#include "ECS/Event.h"

namespace dbg::level
{
	struct SaveRequest final : public ecs::Event<SaveRequest> { };
}
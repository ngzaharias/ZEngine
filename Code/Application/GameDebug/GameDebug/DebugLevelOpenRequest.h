#pragma once

#include "ECS/Event.h"

namespace dbg::level
{
	struct OpenRequest final : public ecs::Event<OpenRequest> { };
}
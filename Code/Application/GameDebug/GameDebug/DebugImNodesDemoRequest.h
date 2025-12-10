#pragma once

#include "ECS/Event.h"

namespace dbg
{
	struct ImNodesDemoRequest final : public ecs::Event<ImNodesDemoRequest> { };
}
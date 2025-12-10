#pragma once

#include "ECS/Event.h"

namespace debug
{
	struct ImNodesDemoRequest final : public ecs::Event<ImNodesDemoRequest> { };
}
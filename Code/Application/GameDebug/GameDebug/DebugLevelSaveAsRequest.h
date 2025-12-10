#pragma once

#include "ECS/Event.h"

namespace dbg::level
{
	struct SaveAsRequest final : public ecs::Event<SaveAsRequest> { };
}
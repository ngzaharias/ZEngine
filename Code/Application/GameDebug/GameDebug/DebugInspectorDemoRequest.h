#pragma once

#include "ECS/Event.h"

namespace dbg
{
	struct InspectorDemoRequest final : public ecs::Event<InspectorDemoRequest> { };
}
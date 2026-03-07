#pragma once

#include "ECS/Event.h"

namespace debug
{
	struct InspectorDemoEvent final : public ecs::Event<InspectorDemoEvent> { };
}
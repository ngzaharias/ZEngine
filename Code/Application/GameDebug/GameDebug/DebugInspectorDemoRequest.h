#pragma once

#include "ECS/Event.h"

namespace debug
{
	struct InspectorDemoRequest final : public ecs::Event<InspectorDemoRequest> { };
}
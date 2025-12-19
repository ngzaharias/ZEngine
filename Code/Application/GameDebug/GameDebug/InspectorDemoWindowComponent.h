#pragma once

#include "ECS/Component.h"

namespace debug
{
	struct InspectorDemoWindowComponent final : public ecs::Component<InspectorDemoWindowComponent> { };
};
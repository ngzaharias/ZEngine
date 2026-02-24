#pragma once

#include "ECS/Component.h"
#include "ECS/IsPrototype.h"

namespace gui
{
	struct HUDComponent final : public ecs::Component<HUDComponent>
		, ecs::IsPrototype
	{
	};
}

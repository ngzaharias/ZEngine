#pragma once

#include "ECS/Component.h"

namespace tactics
{
	struct SelectedComponent : ecs::Component<SelectedComponent>
	{
	};
}
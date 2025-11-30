#pragma once

#include "ECS/Component.h"

namespace tactics
{
	struct SelectedComponent final : public ecs::Component<SelectedComponent>
	{
	};
}
#pragma once

#include "ECS/Component.h"

namespace drag
{
	struct IsSelectableComponent final : public ecs::Component<IsSelectableComponent> {};
}
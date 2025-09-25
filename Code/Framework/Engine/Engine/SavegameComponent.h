#pragma once

#include "ECS/Component.h"

namespace eng
{
	struct SavegameComponent final : public ecs::Component<SavegameComponent> {};
}
#pragma once

#include "ECS/Component.h"

namespace eng
{
	struct TablesReloaded final : public ecs::Event<TablesReloaded> {};
}
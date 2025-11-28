#pragma once

#include "ECS/Component.h"

namespace eng
{
	struct TablesReloadedEvent final : ecs::Event<TablesReloadedEvent> {};
}
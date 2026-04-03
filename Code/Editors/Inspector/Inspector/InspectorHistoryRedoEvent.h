#pragma once

#include "ECS/Event.h"

namespace editor::inspector
{
	struct HistoryRedoEvent final : public ecs::Event { };
}
#pragma once

#include "ECS/Event.h"

namespace editor::inspector
{
	struct HistoryUndoEvent final : public ecs::Event { };
}
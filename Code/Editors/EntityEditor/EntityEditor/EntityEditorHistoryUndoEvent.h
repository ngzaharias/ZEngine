#pragma once

#include "ECS/Event.h"

namespace editor::entity
{
	struct HistoryUndoEvent final : public ecs::Event { };
}
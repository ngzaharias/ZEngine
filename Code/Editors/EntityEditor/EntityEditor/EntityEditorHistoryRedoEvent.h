#pragma once

#include "ECS/Event.h"

namespace editor::entity
{
	struct HistoryRedoEvent final : public ecs::Event { };
}
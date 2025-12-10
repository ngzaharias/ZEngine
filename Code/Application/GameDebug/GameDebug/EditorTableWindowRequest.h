#pragma once

#include "ECS/Event.h"

namespace editor
{
	struct TableWindowRequest final : public ecs::Event<TableWindowRequest> { };
}
#pragma once

#include "ECS/Event.h"

namespace editor
{
	struct TrajectoryWindowRequest final : public ecs::Event<TrajectoryWindowRequest> { };
}
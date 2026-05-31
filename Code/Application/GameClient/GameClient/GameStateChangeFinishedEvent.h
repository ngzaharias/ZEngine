#pragma once

#include "ECS/Event.h"

namespace shared::gamestate
{
	/// \brief Signals that the current gamestate has finished.
	struct ChangeFinishedEvent final : public ecs::Event { };
}
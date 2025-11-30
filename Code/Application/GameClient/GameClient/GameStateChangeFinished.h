#pragma once

#include "ECS/Event.h"

namespace gamestate
{
	/// \brief Signals that the current gamestate has finished.
	struct ChangeFinished final : public ecs::Event<ChangeFinished> { };
}
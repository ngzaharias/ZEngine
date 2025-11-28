#pragma once

#include "ECS/Event.h"

namespace eng::network
{
	/// \brief 
	struct ChangeFinishedEvent final : public ecs::Event<ChangeFinishedEvent> { };
}
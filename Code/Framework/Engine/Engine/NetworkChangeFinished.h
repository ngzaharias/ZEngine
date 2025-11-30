#pragma once

#include "ECS/Event.h"

namespace eng::network
{
	/// \brief 
	struct ChangeFinished final : public ecs::Event<ChangeFinished> { };
}
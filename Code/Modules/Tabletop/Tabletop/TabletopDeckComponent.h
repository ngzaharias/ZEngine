#pragma once

#include "ECS/Component.h"
#include "ECS/IsPrototype.h"

namespace tabletop
{
	/// \brief 
	struct DeckComponent final : public ecs::Component<DeckComponent>
		, ecs::IsPrototype
	{
	};
}
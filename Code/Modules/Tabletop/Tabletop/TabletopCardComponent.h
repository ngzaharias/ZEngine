#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"
#include "ECS/IsPrototype.h"

namespace tabletop
{
	/// \brief 
	struct CardComponent final : public ecs::Component<CardComponent>
		, ecs::IsPrototype
	{
	};
}
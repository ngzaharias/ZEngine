#pragma once

#include "ECS/Component.h"
#include "ECS/IsPrototype.h"

namespace camera
{
	/// \brief
	struct Move2DComponent final : public ecs::Component<Move2DComponent>
		, ecs::IsPrototype
	{
	};
}
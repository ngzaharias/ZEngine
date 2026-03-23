#pragma once

#include "ECS/Component.h"
#include "ECS/IsPrototype.h"

namespace camera
{
	/// \brief
	struct Pan3DComponent final : public ecs::Component
		, ecs::IsPrototype
	{
	};
}
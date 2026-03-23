#pragma once

#include "ECS/Component.h"
#include "ECS/IsPrototype.h"
#include "Math/Vector.h"

namespace eng::light
{
	struct AmbientComponent final : public ecs::Component
		, ecs::IsPrototype
	{
		Vector3f m_Colour = Vector3f::One;
	};
}
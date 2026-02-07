#pragma once

#include "ECS/Component.h"
#include "ECS/IsPrototype.h"
#include "Math/Vector.h"

namespace eng::light
{
	struct PointComponent final : public ecs::Component<PointComponent>
		, ecs::IsPrototype
	{
		Vector3f m_Colour = Vector3f::One;
		float m_Range = 500.f;
	};
}
#pragma once

#include "ECS/Component.h"
#include "Math/Vector.h"

namespace eng::light
{
	struct PointComponent final : public ecs::Component<PointComponent>
	{
		Vector3f m_Colour = Vector3f::One;
		float m_Range = 500.f;
	};
}
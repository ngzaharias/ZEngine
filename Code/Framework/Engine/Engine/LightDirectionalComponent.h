#pragma once

#include "ECS/Component.h"
#include "Math/Vector.h"

namespace eng::light
{
	struct DirectionalComponent final : public ecs::Component
	{
		Vector3f m_Colour = Vector3f::One;
	};
}
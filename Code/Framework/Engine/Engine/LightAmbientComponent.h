#pragma once

#include "ECS/Component.h"
#include "Math/Vector.h"

namespace eng::light
{
	struct AmbientComponent final : public ecs::Component<AmbientComponent>
	{
		Vector3f m_Colour = Vector3f::One;
	};
}
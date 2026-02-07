#pragma once

#include "ECS/Component.h"
#include "ECS/IsPrototype.h"
#include "Math/Vector.h"

namespace eng::light
{
	struct DirectionalComponent final : public ecs::Component<DirectionalComponent>
		, ecs::IsPrototype
	{
		Vector3f m_Colour = Vector3f::One;
	};
}
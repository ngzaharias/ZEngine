#pragma once

#include "ECS/Component.h"
#include "ECS/IsPrototype.h"
#include "Math/Math.h"
#include "Math/Vector.h"

namespace camera
{
	/// \brief
	struct Bound2DComponent final : public ecs::Component<Bound2DComponent>
		, ecs::IsPrototype
	{
		Vector2f m_Max = Vector2f(+KINDA_LARGE_FLOAT);
		Vector2f m_Min = Vector2f(-KINDA_LARGE_FLOAT);
	};
}
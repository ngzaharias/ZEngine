#pragma once

#include "ECS/Component.h"
#include "ECS/IsPrototype.h"
#include "Math/Vector.h"

namespace tilemap
{
	struct GridComponent final : public ecs::Component<GridComponent>
		, ecs::IsPrototype
	{
		Vector3i m_GridSize = Vector3i::Zero;
		Vector3f m_TileSize = Vector3f::Zero;
	};
}
#pragma once

#include "ECS/Component.h"
#include "Math/Vector.h"

namespace tilemap
{
	struct GridComponent final : public ecs::Component<GridComponent>
	{
		Vector3i m_GridSize = Vector3i::Zero;
		Vector3f m_TileSize = Vector3f::Zero;
	};
}
#pragma once

#include "ECS/Component.h"
#include "Math/Vector.h"

namespace tilemap
{
	struct GridTemplate final : public ecs::TemplateComponent
	{
		Vector3i m_GridSize = Vector3i::Zero;
		Vector3f m_TileSize = Vector3f::Zero;
	};
}
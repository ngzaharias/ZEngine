#pragma once

#include "Core/GUID.h"
#include "ECS/Component.h"
#include "Math/AABB.h"
#include "GameClient/HexmapData.h"

namespace hexmap
{
	struct RootComponent : ecs::Component<RootComponent>
	{
		int32 m_DepthMin = -INT32_MAX;
		int32 m_DepthMax = +INT32_MAX;

		int32 m_HexRatio = 3;
		float m_HexRadius = 100.f;
		Vector2i m_HexCount = Vector2i(20, 10);

		int32 m_Depth = 0;
		float m_Zoom = 0.5f;
		AABB2f m_Zone = {};
	};
}
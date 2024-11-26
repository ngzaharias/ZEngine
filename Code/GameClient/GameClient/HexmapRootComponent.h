#pragma once

#include "Core/GUID.h"
#include "ECS/Component.h"
#include "Math/AABB.h"
#include "GameClient/HexmapData.h"

namespace hexmap
{
	struct RootComponent : ecs::Component<RootComponent>
	{
		float m_HexRadius = 100.f;
		Vector2i m_HexCount = Vector2i(20, 10);

		float m_Zoom = 0.5f;
		AABB2f m_Zone = {};
	};
}
#pragma once

#include "Core/GUID.h"
#include "ECS/Component.h"
#include "ECS/IsPrototype.h"
#include "Hexmap/HexmapData.h"
#include "Math/AABB.h"

namespace hexmap
{
	struct RootComponent final : public ecs::Component<RootComponent>
		, ecs::IsPrototype
	{
		float m_HexRadius = 100.f;
		Vector2i m_HexCount = Vector2i(20, 10);

		float m_Zoom = 0.5f;
		AABB2f m_Zone = {};
	};
}
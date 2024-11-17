#pragma once

#include "ECS/Component.h"
#include "Math/AABB.h"
#include "Math/Hexagon.h"

namespace hexmap
{
	struct ChartComponent : ecs::SingletonComponent<ChartComponent>
	{
		float m_Level = 0.f;
		Set<hexagon::Offset> m_Loaded = {};
	};
}
#pragma once

#include "ECS/Component.h"
#include "Math/AABB.h"
#include "Math/Hexagon.h"

namespace hexmap
{
	struct Hexagon
	{
		bool operator<(const Hexagon& rhs) const
		{
			if (m_Level != rhs.m_Level)
				return m_Level < rhs.m_Level;
			return m_Offset < rhs.m_Offset;
		}

		int32 m_Level = 0;
		hexagon::Offset m_Offset = {};
	};

	struct ChartComponent : ecs::SingletonComponent<ChartComponent>
	{
		int32 m_Level = 0;
		AABB2f m_Frustrum = {};
		float m_TileRadius = 100.f;

		Set<Hexagon> m_Loaded = {};
	};
}
#pragma once

#include "ECS/Component.h"
#include "Math/AABB.h"

namespace hexmap
{
	struct ChartComponent : ecs::SingletonComponent<ChartComponent>
	{
		float GetRadiusMin() const { return m_Radius; }
		float GetRadiusMax() const { return m_Radius * m_Ratio; }

		float m_Level = 0.f;
		int32 m_Ratio = 6;
		float m_Radius = 100.f;

		AABB2f m_Zone = {};
	};
}
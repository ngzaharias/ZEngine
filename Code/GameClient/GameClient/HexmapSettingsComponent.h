#pragma once

#include "ECS/Component.h"
#include "Math/AABB.h"

namespace hexmap
{
	struct SettingsComponent : ecs::SingletonComponent<SettingsComponent>
	{
		int32 m_TileRatio = 6;
		float m_TileRadius = 100.f;
		Vector2i m_TileCount = Vector2i(20,10);
	};
}
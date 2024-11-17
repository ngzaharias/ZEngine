#pragma once

#include "ECS/Component.h"
#include "Math/AABB.h"

namespace hexmap
{
	struct SettingsComponent : ecs::SingletonComponent<SettingsComponent>
	{
		float GetFragmentRadius() const { return m_TileRadius * m_TileCount; }

		float m_TileRadius = 100.f;
		int32 m_TileCount = 50;
	};
}
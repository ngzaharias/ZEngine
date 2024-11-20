#pragma once

#include "Core/Array.h"
#include "Core/Guid.h"
#include "ECS/Component.h"
#include "Math/Hexagon.h"

namespace hexmap
{
	struct FragmentComponent : ecs::Component<FragmentComponent>
	{
		int32 m_Level = 0;
		float m_TileRadius = 100.f;
		hexagon::Offset m_HexPos = {};

		Vector2i m_TileCount = {};
		Array<int32> m_Data = {};

		// #temp
		str::Guid m_Sprite = {}; 
	};
}
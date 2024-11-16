#pragma once

#include "Core/Array.h"
#include "Core/Guid.h"
#include "ECS/Component.h"
#include "Math/Hexagon.h"

namespace hexmap
{
	struct FragmentComponent : ecs::Component<FragmentComponent>
	{
		int32 GetHeight() const { return m_Dimensions.y; }
		int32 GetWidth() const { return m_Dimensions.x; }

		int32 m_Level = 0;
		float m_Radius = 100.f;
		Vector2i m_Position = {};

		str::Guid m_Sprite = {}; // #temp:

		Vector2i m_Dimensions = {};
		Array<int32> m_Data = {};
	};
}
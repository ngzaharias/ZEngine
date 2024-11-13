#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"
#include "Math/Hexagon.h"

namespace hexmap
{
	struct FragmentComponent : ecs::Component<FragmentComponent>
	{
		int32 m_Level = 0;
		Vector2i m_Position = {};

		AABB2i m_Range = {};
		Array<int32> m_Data = {};
	};
}
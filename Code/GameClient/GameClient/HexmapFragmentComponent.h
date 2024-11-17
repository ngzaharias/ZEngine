#pragma once

#include "Core/Array.h"
#include "Core/Guid.h"
#include "ECS/Component.h"
#include "Math/Hexagon.h"

namespace hexmap
{
	struct FragmentComponent : ecs::Component<FragmentComponent>
	{
		hexagon::Offset m_HexPos = {};
		Array<int32> m_Data = {};

		// #temp
		str::Guid m_Sprite = {}; 
	};
}
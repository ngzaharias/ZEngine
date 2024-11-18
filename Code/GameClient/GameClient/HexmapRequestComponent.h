#pragma once

#include "Core/Set.h"
#include "ECS/Component.h"
#include "Math/Hexagon.h"
#include "GameClient/HexmapChartComponent.h"

namespace hexmap
{
	struct RequestComponent : ecs::EventComponent<RequestComponent>
	{
		Set<Hexagon> m_Load;
		Set<Hexagon> m_Unload;
	};
}
#pragma once

#include "Core/Set.h"
#include "ECS/Component.h"
#include "Math/Hexagon.h"

namespace hexmap
{
	struct RequestComponent : ecs::EventComponent<RequestComponent>
	{
		Set<hexagon::Offset> m_Load;
		Set<hexagon::Offset> m_Unload;
	};
}
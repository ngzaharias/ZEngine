#pragma once

#include "ECS/Component.h"
#include "Math/AABB.h"

namespace hexmap
{
	struct RequestComponent : ecs::EventComponent<RequestComponent>
	{
		AABB2f m_ZoneOld = {};
		AABB2f m_ZoneNew = {};
	};
}
#pragma once

#include "ECS/Component.h"
#include "ECS/IsPrototype.h"

namespace eng
{
	// \brief 
	struct VisibilityComponent final : public ecs::Component
		, ecs::IsPrototype
	{
		bool m_IsVisible = true;
	};
}
#pragma once

#include "ECS/Component.h"

namespace eng
{
	// \brief 
	struct VisibilityComponent final : public ecs::Component
	{
		bool m_IsVisible = true;
	};
}
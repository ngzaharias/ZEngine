#pragma once

#include "ECS/Component.h"

namespace eng
{
	// \brief 
	struct VisibilityComponent : public ecs::Component<VisibilityComponent>
	{
		bool m_IsVisible = true;
	};
}
#pragma once

#include "ECS/Component.h"

namespace eng
{
	// \brief 
	struct VisibilityTemplate final : public ecs::TemplateComponent
	{
		bool m_IsVisible = true;
	};
}
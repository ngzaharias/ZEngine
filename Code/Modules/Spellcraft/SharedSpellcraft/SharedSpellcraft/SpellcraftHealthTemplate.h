#pragma once

#include "ECS/Component.h"

namespace spellcraft
{
	struct HealthTemplate final : public ecs::TemplateComponent
	{
		int32 m_Initial = 0;
		int32 m_Maximum = 0;
	};
}
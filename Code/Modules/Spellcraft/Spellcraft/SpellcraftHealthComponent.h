#pragma once

#include "ECS/Component.h"

namespace spellcraft
{
	struct HealthComponent final : public ecs::Component
	{
		int32 m_Current = 0;
	};
}
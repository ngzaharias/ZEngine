#pragma once

#include "ECS/Component.h"

namespace spellcraft
{
	struct ExplosionComponent final : public ecs::Component
	{
		str::Guid m_OnHit = {};

		float m_Duration = 0.f;
		float m_Radius = 0.f;
		float m_Speed = 0.f;
	};
}
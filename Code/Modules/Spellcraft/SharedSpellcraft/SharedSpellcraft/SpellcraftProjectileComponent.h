#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"
#include "Math/Vector.h"

namespace spellcraft
{
	struct ProjectileComponent final : public ecs::Component
	{
		str::Guid m_OnHit = {};
		
		Vector3f m_Direction = {};
		float m_Distance = 0.f;
		float m_Speed = 0.f;
	};
}
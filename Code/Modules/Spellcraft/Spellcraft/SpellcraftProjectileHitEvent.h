#pragma once

#include "Core/Guid.h"
#include "ECS/Entity.h"
#include "ECS/Event.h"
#include "Math/Vector.h"

namespace spellcraft
{
	struct ProjectileHitEvent final : public ecs::Event
	{
		// entity that was hit by the projectile
		ecs::Entity m_Entity = {};
		str::Guid m_OnHit = {};

		Vector3f m_Translate = {};
		Vector3f m_Direction = {};
	};
}
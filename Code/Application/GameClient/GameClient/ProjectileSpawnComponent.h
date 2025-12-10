#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"

namespace projectile
{
	/// \brief Holds information about the spawned projectile.
	struct SpawnComponent final : public ecs::Component<SpawnComponent>
	{
		// The unique id to track the projectile.
		str::Guid m_TransactionId = { };
		// The owner of the projectile, can be unassigned.
		ecs::Entity m_Owner = { };
		// The timer for how long the projectile has been alive.
		float m_Lifetime = 0.f;
		// The time at which the projectile will be destroyed.
		float m_Timeout = 0.f;
	};
}
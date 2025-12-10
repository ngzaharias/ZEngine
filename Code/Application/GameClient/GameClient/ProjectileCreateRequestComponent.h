#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "GameClient/ProjectileTypes.h"
#include "Math/Vector.h"

namespace projectile
{
	/// \brief Request that a new projectile is created.
	struct CreateRequestComponent final : public ecs::Component<CreateRequestComponent>
	{
		// The unique id to track the entity.
		str::Guid m_TransactionId = { };
		// The owner of the entity, can be unassigned.
		ecs::Entity m_Owner = { };

		LifetimeData m_Lifetime = { };
		TrajectoryData m_Trajectory = { };
		TransformData m_Transform = { };
		VelocityData m_Velocity = { };
		VisualData m_Visual = { };
	};
}
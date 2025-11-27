#pragma once

#include "Core/Trajectory.h"
#include "ECS/Component.h"
#include "GameClient/ProjectileTypes.h"
#include "Math/Vector.h"

namespace projectile
{
	enum class EError
	{
		None,
		// projectile entity is dead
		ProjectileDead,
	};

	struct Created
	{
		ecs::Entity m_Projectile = { };
		ecs::Entity m_Request = { };
	};

	struct Destroyed
	{
		ecs::Entity m_Projectile = { };
	};

	/// \brief 
	struct ChangesSingleton final : public ecs::Singleton<ChangesSingleton>
	{
		Array<Created> m_Created = { };
		Array<Destroyed> m_Destroyed = { };
	};

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

	/// \brief Result of a spawn request made to a storage entity.
	struct CreateResultComponent final : public ecs::Component<CreateResultComponent>
	{
		// The unique id to track the projectile.
		str::Guid m_TransactionId = { };
		// The projectile that was created, unassigned if it failed.
		ecs::Entity m_Projectile = { };
		// Error result when creating the projectile.
		EError m_Error = { };
	};

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

	/// \brief Holds information related to the trajectory of the projectile.
	struct TrajectoryComponent final : public ecs::Component<TrajectoryComponent>
	{
		// The trajectory the projectile will follow.
		path::Trajectory m_Trajectory = { };
		// The distance the projectile has traveled.
		float m_Distance = 0.f;

		// The origin of the trajectory.
		Vector3f m_Origin = Vector3f::Zero;
		// The scale of the trajectory.
		float m_Scale = 1.f;
	};
}
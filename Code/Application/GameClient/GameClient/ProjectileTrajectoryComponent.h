#pragma once

#include "Core/Trajectory.h"
#include "ECS/Component.h"
#include "Math/Vector.h"

namespace projectile
{
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
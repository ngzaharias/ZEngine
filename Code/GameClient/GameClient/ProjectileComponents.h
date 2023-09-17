#pragma once

#include <Core/Rotator.h>
#include <Core/Trajectory.h>
#include <Core/Variant.h>
#include <Core/Vector.h>

#include <ECS/Component.h>

namespace speed
{
	struct Constant 
	{
		float m_Speed = 0.f;
	};

	struct Linear
	{
		float m_Acceleration = 0.f;
		float m_Maximum = 0.f;
		float m_Minimum = 0.f;
		float m_Initial = 0.f;
	};

	using Velocity = Variant<Constant, Linear>;
}

namespace projectile
{
	/// \brief
	struct SpawnRequestComponent final : public ecs::Component<SpawnRequestComponent>
	{
		path::Trajectory m_Trajectory = { };
		speed::Velocity m_Velocity = { };

		Vector3f m_Translate = Vector3f::Zero;
		Rotator m_Rotate = Rotator::Zero;
		Vector3f m_Scale = Vector3f::One;

		float m_Lifetime = 0.f;
	};

	/// \brief
	struct LifetimeComponent final : public ecs::Component<LifetimeComponent>
	{
		float m_Lifetime = 0.f;
	};

	/// \brief
	struct TrajectoryComponent final : public ecs::Component<TrajectoryComponent>
	{
		path::Trajectory m_Trajectory = { };
		Vector3f m_Translate = Vector3f::Zero;
		float m_Distance = 0.f;
	};
}
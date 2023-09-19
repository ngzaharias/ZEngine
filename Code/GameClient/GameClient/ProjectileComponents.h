#pragma once

#include <Core/Rotator.h>
#include <Core/Trajectory.h>
#include <Core/Variant.h>
#include <Core/Vector.h>

#include <ECS/Component.h>

#include <GameClient/ProjectileTypes.h>

namespace projectile
{
	/// \brief
	struct RequestComponent final : public ecs::Component<RequestComponent>
	{
		LifetimeData m_Lifetime = { };
		TrajectoryData m_Trajectory = { };
		TransformData m_Transform = { };
		VelocityData m_Velocity = { };
		VisualData m_Visual = { };
	};

	/// \brief
	struct SettingsComponent final : public ecs::Component<SettingsComponent>
	{
		path::Trajectory m_Trajectory = { };
		speed::Velocity m_Velocity = { };

		Vector3f m_Origin = Vector3f::Zero;
		float m_Lifetime = 0.f;
		float m_Scale = 1.f;
	};

	/// \brief
	struct StateComponent final : public ecs::Component<StateComponent>
	{
		float m_Distance = 0.f;
		float m_Lifetime = 0.f;
		float m_Velocity = 0.f;
	};
}
#pragma once

#include <Core/Guid.h>
#include <Core/Rotator.h>
#include <Core/Trajectory.h>
#include <Core/Variant.h>
#include <Core/Vector.h>

namespace speed
{
	struct Constant 
	{
		float m_Speed = 0.f;
	};

	struct Linear
	{
		float m_Increase = 0.f;
		float m_Initial = 0.f;
		float m_Maximum = 0.f;
		float m_Minimum = 0.f;
	};

	using Velocity = Variant<speed::Constant, speed::Linear>;
}

namespace projectile
{
	struct LifetimeData
	{
		float m_Lifetime = 0.f;
	};

	struct TrajectoryData
	{
		path::Trajectory m_Trajectory;
		Vector3f m_Origin = Vector3f::Zero;
		float m_Scale = 1.f;
	};

	struct TransformData
	{
		ecs::Entity m_Parent = { };
		Vector3f m_Translate = Vector3f::Zero;
		Rotator m_Rotate = Rotator::Zero;
		Vector3f m_Scale = Vector3f::One;
	};

	struct VelocityData
	{
		speed::Velocity m_Velocity = { };
	};

	struct VisualData
	{
		str::Guid m_StaticMesh = { };
	};
}
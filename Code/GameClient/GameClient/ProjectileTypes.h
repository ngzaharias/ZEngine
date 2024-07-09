#pragma once

#include "Core/Guid.h"
#include "Core/Nullable.h"
#include "Core/Rotator.h"
#include "Core/Trajectory.h"
#include "Core/Variant.h"
#include "Core/Vector.h"
#include "ECS/Entity.h"

namespace projectile
{
	struct LifetimeData
	{
		float m_Timeout = 0.f;
	};

	struct TrajectoryData
	{
		path::Trajectory m_Trajectory;
		Vector3f m_Origin = Vector3f::Zero;
		float m_Scale = 1.f;
	};

	struct TransformData
	{
		Vector3f m_Translate = Vector3f::Zero;
		Rotator m_Rotate = Rotator::Zero;
		Vector3f m_Scale = Vector3f::One;
	};

	struct VelocityData
	{
		float m_Acceleration = 0.f;
		float m_Initial = 0.f;
		float m_Maximum = 0.f;
		float m_Minimum = 0.f;
	};

	struct VisualData
	{
		str::Guid m_StaticMesh = { };
	};
}
#pragma once

#include <Core/Vector.h>

#include <ECS/Component.h>

namespace movement
{
	/// \brief
	struct AccelerationComponent final : public ecs::Component<AccelerationComponent>
	{
		float m_Maximum = +FLT_MAX;
		float m_Minimum = -FLT_MAX;
		float m_Speed = 0.f;
	};

	/// \brief
	struct VelocityComponent final : public ecs::Component<VelocityComponent>
	{
		Vector3f m_Direction = Vector3f::AxisZ;
		float m_Speed = 0.f;
	};
}
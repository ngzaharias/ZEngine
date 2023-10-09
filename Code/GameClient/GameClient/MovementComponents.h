#pragma once

#include <Core/Vector.h>

#include <ECS/Component.h>

namespace movement
{
	/// \brief
	struct AccelerationComponent final : public ecs::Component<AccelerationComponent>
	{
		float m_Acceleration = 0.f;
		float m_Maximum = 0.f;
		float m_Minimum = 0.f;
	};

	/// \brief
	struct VelocityComponent final : public ecs::Component<VelocityComponent>
	{
		Vector3f m_Direction = Vector3f::Zero;
		float m_Speed = 0.f;
	};
}
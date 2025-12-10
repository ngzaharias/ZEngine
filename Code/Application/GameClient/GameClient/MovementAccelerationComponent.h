#pragma once

#include "ECS/Component.h"

namespace movement
{
	/// \brief
	struct AccelerationComponent final : public ecs::Component<AccelerationComponent>
	{
		float m_Acceleration = 0.f;
		float m_Maximum = 0.f;
		float m_Minimum = 0.f;
	};
}
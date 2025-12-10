#pragma once

#include "ECS/Component.h"
#include "Math/Vector.h"

namespace movement
{
	/// \brief
	struct VelocityComponent final : public ecs::Component<VelocityComponent>
	{
		Vector3f m_Direction = Vector3f::Zero;
		float m_Speed = 0.f;
	};
}
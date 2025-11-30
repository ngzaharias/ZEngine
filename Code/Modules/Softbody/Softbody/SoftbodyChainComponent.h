#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"
#include "Math/Vector.h"

namespace softbody
{
	struct Link
	{
		Vector2f m_Direction = Vector2f::AxisY;
		Vector2f m_Position = Vector2f::Zero;
	};

	struct ChainComponent final : public ecs::Component<ChainComponent>
	{
		// constraints
		float m_Angle = math::ToRadians(0.f);
		float m_Radius = 200.f;
		Vector2f m_Gravity = Vector2f::Zero;

		Array<Link> m_Links = { 
			Link{}, Link{}, Link{}, Link{} };
	};
}
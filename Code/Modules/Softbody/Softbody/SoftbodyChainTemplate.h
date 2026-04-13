#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"
#include "Math/Vector.h"

namespace softbody
{
	struct ChainTemplate final : public ecs::TemplateComponent
	{
		// constraints
		float m_Angle = math::ToRadians(0.f);
		float m_Radius = 200.f;
		Vector2f m_Gravity = Vector2f::Zero;
	};
}
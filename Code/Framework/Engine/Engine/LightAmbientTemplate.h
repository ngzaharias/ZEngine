#pragma once

#include "ECS/Component.h"
#include "Math/Vector.h"

namespace eng::light
{
	struct AmbientTemplate final : public ecs::TemplateComponent
	{
		Vector3f m_Colour = Vector3f::One;
	};
}
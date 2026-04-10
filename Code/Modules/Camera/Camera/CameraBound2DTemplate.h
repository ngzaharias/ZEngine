#pragma once

#include "ECS/Component.h"
#include "Math/Math.h"
#include "Math/Vector.h"

namespace camera
{
	/// \brief
	struct Bound2DTemplate final : public ecs::TemplateComponent
	{
		Vector2f m_Max = Vector2f(+KINDA_LARGE_FLOAT);
		Vector2f m_Min = Vector2f(-KINDA_LARGE_FLOAT);
	};
}
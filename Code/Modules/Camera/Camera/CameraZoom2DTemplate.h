#pragma once

#include "Core/Optional.h"
#include "ECS/Component.h"
#include "Math/Math.h"
#include "Math/Vector.h"

namespace camera
{
	/// \brief
	struct Zoom2DTemplate final : public ecs::TemplateComponent
	{
		float m_Max = KINDA_LARGE_FLOAT;
		float m_Min = 1.f;
	};
}
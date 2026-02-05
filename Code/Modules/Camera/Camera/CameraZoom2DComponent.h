#pragma once

#include "Core/Optional.h"
#include "ECS/Component.h"
#include "ECS/IsPrototype.h"
#include "Math/Math.h"
#include "Math/Vector.h"

namespace camera
{
	struct Target
	{
		Vector2f m_Position = Vector2f::Zero;
		float m_Size = 0.f;
	};

	/// \brief
	struct Zoom2DComponent final : public ecs::Component<Zoom2DComponent>
		, ecs::IsPrototype
	{
		float m_Max = KINDA_LARGE_FLOAT;
		float m_Min = 1.f;

		Optional<Target> m_Target = {};
	};
}
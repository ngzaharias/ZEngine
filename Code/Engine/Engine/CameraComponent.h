#pragma once

#include "Core/Optional.h"
#include "Core/Variant.h"
#include "ECS/Component.h"
#include "Engine/CameraTypes.h"
#include "Math/Math.h"
#include "Math/Vector.h"

namespace eng::camera
{
	/// \brief
	struct Bound2DComponent final : public ecs::Component<Bound2DComponent>
	{
		Vector2f m_Max = Vector2f(+KINDA_LARGE_FLOAT);
		Vector2f m_Min = Vector2f(-KINDA_LARGE_FLOAT);
	};

	/// \brief
	struct Move2DComponent final : public ecs::Component<Move2DComponent>
	{
	};

	/// \brief
	struct Move3DComponent final : public ecs::Component<Move3DComponent>
	{
	};

	/// \brief
	struct Pan3DComponent final : public ecs::Component<Pan3DComponent>
	{
	};

	/// \brief
	struct Zoom2DComponent final : public ecs::Component<Zoom2DComponent>
	{
		struct Target
		{
			float m_Size = 0.f;
			Vector2f m_Position = Vector2f::Zero;
		};

		float m_Max = KINDA_LARGE_FLOAT;
		float m_Min = 1.f;

		Optional<Target> m_Target = {};
	};

	/// \brief
	struct ProjectionComponent final : public ecs::Component<ProjectionComponent>
	{
		eng::camera::Projection m_Projection = eng::camera::Perspective();
	};
}
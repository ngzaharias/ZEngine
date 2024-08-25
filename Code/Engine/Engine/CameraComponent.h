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
	struct BehaviourComponent : public ecs::Component<BehaviourComponent>
	{
		Optional<BehaviourBound> m_Bound = std::nullopt;
		Optional<BehaviourMove> m_Move = std::nullopt;
		Optional<BehaviourZoom> m_Zoom = std::nullopt;
	};

	/// \brief
	struct ProjectionComponent : public ecs::Component<ProjectionComponent>
	{
		eng::camera::Projection m_Projection = eng::camera::Perspective();
	};
}
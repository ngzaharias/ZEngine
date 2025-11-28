#pragma once

#include "ECS/Component.h"
#include "Engine/CameraTypes.h"

namespace eng::camera
{
	/// \brief
	struct ProjectionComponent final : public ecs::Component<ProjectionComponent>
	{
		eng::camera::Projection m_Projection = eng::camera::Perspective();
	};
}
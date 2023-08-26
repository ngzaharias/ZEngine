#pragma once

#include <Core/Variant.h>

#include <ECS/Component.h>

#include <Engine/CameraTypes.h>

namespace eng
{
	struct CameraComponent : public ecs::Component<CameraComponent>
	{
		camera::Projection m_Projection = camera::Perspective();
	};
}
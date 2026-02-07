#pragma once

#include "ECS/Component.h"
#include "ECS/IsPrototype.h"
#include "Engine/CameraTypes.h"

namespace eng
{
	/// \brief
	struct CameraComponent final : public ecs::Component<CameraComponent>
		, ecs::IsPrototype
	{
		eng::Projection m_Projection = eng::Perspective();
	};

	struct ActiveComponent  final : public ecs::Component<ActiveComponent>
	{
	};

	struct EditorComponent  final : public ecs::Component<EditorComponent>
	{
	};
}
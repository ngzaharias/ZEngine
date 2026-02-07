#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"
#include "ECS/IsPrototype.h"

namespace eng
{
	struct StaticMeshComponent final : public ecs::Component<StaticMeshComponent>
		, ecs::IsPrototype
	{
		str::Guid m_StaticMesh = { };
	};
}
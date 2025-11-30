#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"

namespace eng
{
	struct StaticMeshComponent final : public ecs::Component<StaticMeshComponent>
	{
		str::Guid m_StaticMesh = { };
	};
}
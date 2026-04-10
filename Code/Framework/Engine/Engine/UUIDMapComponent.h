#pragma once

#include "Core/Guid.h"
#include "Core/Map.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"

namespace eng
{
	struct UUIDMapComponent final : public ecs::StaticComponent
	{
		Map<str::Guid, ecs::Entity> m_ToEntity = {};
	};
}
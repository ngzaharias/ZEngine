#pragma once

#include "Core/Guid.h"
#include "Core/Map.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"

namespace eng
{
	struct TemplateMapComponent final : public ecs::StaticComponent
	{
		Map<str::Guid, ecs::Entity> m_GuidToEntity = {};
	};
}
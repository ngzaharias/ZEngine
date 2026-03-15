#pragma once

#include "ECS/Entity.h"
#include "ECS/Event.h"

namespace editor::sprite
{
	struct AssetSaveEvent final : public ecs::Event<AssetSaveEvent> 
	{
		AssetSaveEvent() = default;
		AssetSaveEvent(const ecs::Entity& entity) : m_Entity(entity) {}

		ecs::Entity m_Entity = {};
	};
}

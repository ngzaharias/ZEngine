#pragma once

#include "ECS/Entity.h"
#include "ECS/Event.h"

namespace editor::sprite
{
	struct AssetNewEvent final : public ecs::Event<AssetNewEvent> 
	{ 
		AssetNewEvent() = default;
		AssetNewEvent(const ecs::Entity& entity) : m_Entity(entity) {}

		ecs::Entity m_Entity = {};
	};
}

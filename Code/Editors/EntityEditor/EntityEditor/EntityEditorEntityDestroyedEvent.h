#pragma once

#include "ECS/Entity.h"
#include "ECS/Event.h"

namespace editor::entity
{
	struct EntityDestroyedEvent final : public ecs::Event
	{
		EntityDestroyedEvent() = default;
		EntityDestroyedEvent(const ecs::Entity& entity) : m_Entity(entity) {}

		ecs::Entity m_Entity = {};
	};
}
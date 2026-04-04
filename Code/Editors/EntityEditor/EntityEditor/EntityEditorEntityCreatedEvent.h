#pragma once

#include "ECS/Entity.h"
#include "ECS/Event.h"

namespace editor::entity
{
	struct EntityCreatedEvent final : public ecs::Event 
	{ 
		EntityCreatedEvent() = default;
		EntityCreatedEvent(const ecs::Entity& entity) : m_Entity(entity) {}

		ecs::Entity m_Entity = {};
	};
}
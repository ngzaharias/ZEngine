#pragma once

#include "ECS/Entity.h"
#include "ECS/Event.h"

namespace editor::entity
{
	struct EntitySelectedEvent final : public ecs::Event
	{
		EntitySelectedEvent() = default;
		EntitySelectedEvent(const ecs::Entity& before, const ecs::Entity& after)
			: m_Before(before), m_After(after) { }

		ecs::Entity m_Before = {};
		ecs::Entity m_After = {};
	};
}
#pragma once

#include "Core/String.h"
#include "ECS/Entity.h"
#include "ECS/Event.h"

namespace editor::entity
{
	/// \brief Event to signal that an entity was updated (components added/removed/updated).
	struct EntityUpdatedEvent final : public ecs::Event
	{
		EntityUpdatedEvent() = default;
		EntityUpdatedEvent(const ecs::Entity& entity, const str::String& data)
			: m_Entity(entity), m_Data(data) { }
		EntityUpdatedEvent(const ecs::Entity& entity, str::String&& data)
			: m_Entity(entity), m_Data(data) { }

		// data from before the entity was updated
		ecs::Entity m_Entity = {};
		str::String m_Data = {}; 
	};
}
#pragma once

#include "ECS/Entity.h"
#include "ECS/Event.h"

namespace editor::sprite
{
	struct AssetOpenEvent final : public ecs::Event 
	{
		AssetOpenEvent() = default;
		AssetOpenEvent(const ecs::Entity& entity) : m_Entity(entity) {}

		ecs::Entity m_Entity = {};
	};
}

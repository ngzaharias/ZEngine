#pragma once

#include "ECS/Entity.h"
#include "ECS/Event.h"

namespace editor::inspector
{
	struct HistoryDestroyEvent final : public ecs::Event
	{
		ecs::Entity m_Entity = {};
	};
}
#pragma once

#include "ECS/Entity.h"
#include "ECS/Event.h"

namespace editor::inspector
{
	struct HistoryCreateEvent final : public ecs::Event 
	{ 
		ecs::Entity m_Entity = {};
	};
}
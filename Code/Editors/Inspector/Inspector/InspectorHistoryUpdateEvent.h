#pragma once

#include "Core/String.h"
#include "ECS/Entity.h"
#include "ECS/Event.h"

namespace editor::inspector
{
	struct HistoryUpdateEvent final : public ecs::Event
	{
		ecs::Entity m_Entity = {};
		str::String m_Data = {};
	};
}
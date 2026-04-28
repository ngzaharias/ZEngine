#pragma once

#include "ECS/Entity.h"
#include "ECS/Event.h"

namespace editor::spell
{
	struct LinkCreateEvent final : public ecs::Event 
	{
		ecs::Entity m_Entity = {};
		int32 m_SourceId = -1;
		int32 m_TargetId = -1;

	};
}
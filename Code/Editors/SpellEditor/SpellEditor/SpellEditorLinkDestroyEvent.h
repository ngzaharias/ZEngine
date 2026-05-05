#pragma once

#include "ECS/Entity.h"
#include "ECS/Event.h"

namespace editor::spell
{
	struct LinkDestroyEvent final : public ecs::Event
	{
		ecs::Entity m_Entity = {};
		int32 m_LinkId = -1;
	};
}
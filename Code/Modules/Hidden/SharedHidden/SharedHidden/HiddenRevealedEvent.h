#pragma once

#include "Core/Guid.h"
#include "ECS/Event.h"
#include "ECS/IsReplicated.h"

namespace shared::hidden
{
	struct RevealedEvent final : public ecs::Event
		, ecs::IsReplicated
	{
		str::Guid m_Entity = {};
	};
}
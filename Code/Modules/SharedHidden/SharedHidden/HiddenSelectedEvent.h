#pragma once

#include "Core/Guid.h"
#include "ECS/Event.h"
#include "ECS/IsReplicated.h"

namespace shared::hidden
{
	struct SelectedEvent final : public ecs::Event<SelectedEvent>
		, ecs::IsReplicated
	{
		str::Guid m_Entity = {};
	};
}
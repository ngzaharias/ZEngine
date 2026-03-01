#pragma once

#include "Core/Name.h"
#include "ECS/Entity.h"
#include "ECS/Event.h"

namespace tactics
{
	struct ExecuteEvent final : public ecs::Event<ExecuteEvent>
	{
		ecs::Entity m_Entity = {};
		str::Name m_Ability = {};
	};
}
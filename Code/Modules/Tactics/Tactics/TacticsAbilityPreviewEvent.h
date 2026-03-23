#pragma once

#include "Core/Name.h"
#include "ECS/Entity.h"
#include "ECS/Event.h"

namespace tactics
{
	struct AbilityPreviewEvent final : public ecs::Event
	{
		ecs::Entity m_Entity = {};
		str::Name m_Ability = {};
	};
}
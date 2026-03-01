#pragma once

#include "Core/Name.h"
#include "ECS/Entity.h"
#include "ECS/Event.h"

namespace tactics
{
	struct PreviewEvent final : public ecs::Event<PreviewEvent>
	{
		ecs::Entity m_Entity = {};
		str::Name m_Ability = {};
	};
}
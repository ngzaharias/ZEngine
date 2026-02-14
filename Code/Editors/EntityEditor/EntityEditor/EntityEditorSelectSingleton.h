#pragma once

#include "ECS/Entity.h"
#include "ECS/Singleton.h"

namespace editor::entity
{
	struct SelectSingleton final : public ecs::Singleton<SelectSingleton>
	{
		ecs::Entity m_Entity = {};
	};
}
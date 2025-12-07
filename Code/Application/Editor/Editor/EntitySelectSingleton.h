#pragma once

#include "ECS/Entity.h"
#include "ECS/Singleton.h"

namespace editor
{
	struct EntitySelectSingleton final : public ecs::Singleton<EntitySelectSingleton>
	{
		ecs::Entity m_Entity = {};
	};
}